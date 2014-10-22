/***************************************************************************
 *   Copyright (C) 2013-2014 by Tomasz Bojczuk                             *
 *   tomaszbojczuk@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#include "tscorescene.h"
#include "tnotecontrol.h"
#include <graphics/tdropshadoweffect.h>
#include <tnoofont.h>
#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QApplication>
#include <QTimer>


TscoreScene::TscoreScene(QObject* parent) :
  QGraphicsScene(parent),
  m_workPosY(0),
  m_workNote(0),
  m_workAccid(0),
  m_nameColor(Qt::darkCyan),
  m_rightBox(0), m_leftBox(0),
  m_accidYoffset(0.0),
  m_accidScale(-1.0),
  m_scoreNote(0),
  m_controlledNotes(true)
{
	m_showTimer = new QTimer(this);
	m_hideTimer = new QTimer(this);
  setDoubleAccidsEnabled(true);
  m_currentAccid = 0;
	
	connect(m_showTimer, SIGNAL(timeout()), this, SLOT(showTimeOut()));
	connect(m_hideTimer, SIGNAL(timeout()), this, SLOT(hideTimeOut()));
}


TscoreScene::~TscoreScene() 
{
	if (m_rightBox) { // all items are into scene so they will be deleted
		delete m_rightBox; // but the last TscoreNote has to skip deleting depending items itself
		m_rightBox = 0;
	}
}


void TscoreScene::setCurrentAccid(char accid) {
	char prevAcc = m_currentAccid;
	m_currentAccid = (char)qBound((int)-m_dblAccFuse, (int)accid, (int)m_dblAccFuse);
	if (m_workAccid && prevAcc != m_currentAccid) {
		m_workAccid->setText(TscoreNote::getAccid(m_currentAccid));
		if (m_currentAccid == 0)
			m_workAccid->hide();
		else
			m_workAccid->show();
		if (m_leftBox)
			m_leftBox->setAccidental(m_currentAccid);
	}
}


void TscoreScene::setDoubleAccidsEnabled(bool enable) {
  if (enable)
    m_dblAccFuse = 2;
  else
    m_dblAccFuse = 1;
}


void TscoreScene::addBlur(QGraphicsItem* item, qreal radius) {
  QGraphicsBlurEffect *blur = new QGraphicsBlurEffect();
  blur->setBlurRadius(radius / views()[0]->transform().m11());
  item->setGraphicsEffect(blur);
}


void TscoreScene::adjustCursor(TscoreNote* sn) {
	if (m_rightBox && !views().isEmpty()) {
		m_rightBox->adjustSize();
		m_leftBox->adjustSize();
		workLines()->adjustLines(sn);
		setPointedColor(workColor);
	}
}


void TscoreScene::setPointedColor(const QColor& color) {
	workColor = color;
	m_workNote->setPen(QPen(workColor, 0.2));
	m_workNote->setBrush(QBrush(workColor, Qt::SolidPattern));
	m_workAccid->setBrush(QBrush(workColor));
	m_workLines->setColor(color);
}

//##########################################################################################
//#######################        Note CURSOR     ###########################################
//##########################################################################################

void TscoreScene::noteEntered(TscoreNote* sn) {
	if (sn != m_scoreNote && sn != 0) {
		m_scoreNote = sn;
		if (controlledNotes()) {
			if (right()->isEnabled()) {
				right()->setPos(sn->pos().x() + sn->boundingRect().width(), 0.0);
				right()->setScoreNote(sn);
			}
			if (left()->isEnabled()) {
				left()->setPos(sn->pos().x() - left()->boundingRect().width(), 0.0);
				left()->setScoreNote(sn);
			}
		}
		if (workNote()->parentItem() != sn)
				setCursorParent(sn);
	}
}


void TscoreScene::noteMoved(TscoreNote* sn, int yPos) {
	setWorkPosY(yPos);
	workNote()->setPos(3.0, workPosY());
	workLines()->checkLines(yPos);
	if (!workNote()->isVisible()) {
		showTimeOut();
	}
	if (sn != m_scoreNote) {
			noteEntered(sn);
			m_showTimer->start(300);
	} else {
			m_hideTimer->start(1000);
	}
}


void TscoreScene::noteLeaved(TscoreNote* sn) {
	Q_UNUSED(sn)
	m_showTimer->stop();
	m_hideTimer->start(300);
}


void TscoreScene::noteDeleted(TscoreNote* sn) {
	if (right() && (workNote()->parentItem() == sn || right()->parentItem() == sn->parentItem())) {
		right()->setScoreNote(0);
		left()->setScoreNote(0);
		setCursorParent(0);
		hideTimeOut();
	}
	m_scoreNote = 0;
}


void TscoreScene::controlMoved() {
	m_hideTimer->start(1000);
}


//##########################################################################################
//#######################        PROTECTED       ###########################################
//##########################################################################################

void TscoreScene::initNoteCursor(TscoreNote* scoreNote) {
	if (!m_workNote) {
		m_workLines = new TscoreLines(scoreNote);
		workColor = qApp->palette().highlight().color();
		workColor.setAlpha(200);
		m_workNote = TscoreNote::createNoteHead(scoreNote);
		m_workNote->setGraphicsEffect(new TdropShadowEffect(workColor));
		m_workAccid = new QGraphicsSimpleTextItem();
		m_workAccid->setBrush(QBrush(workColor));
		m_workAccid->setParentItem(m_workNote);
		m_workAccid->hide();
		m_workAccid->setFont(TnooFont(5));
		m_workAccid->hide();
		m_workAccid->setScale(accidScale());
		m_workAccid->setPos(-3.0, - accidYoffset());
		m_workNote->setZValue(35);
		m_workAccid->setZValue(m_workNote->zValue());
		setPointedColor(workColor);
		
		m_rightBox = new TnoteControl(scoreNote->staff(), this);
		m_leftBox = new TnoteControl(scoreNote->staff(), this);
		m_leftBox->addAccidentals();
	}
}


void TscoreScene::setCursorParent(TscoreItem* item) {
	workNote()->setParentItem(item);
	m_workLines->setParent(item);
}


//##########################################################################################
//#######################     PROTECTED SLOTS    ###########################################
//##########################################################################################

void TscoreScene::showTimeOut() {
	m_showTimer->stop();
	m_workNote->show();
	m_workAccid->show();
	if (left()->isEnabled())
		left()->show();
	if (right()->isEnabled())
		right()->show();
}


void TscoreScene::hideTimeOut() {
	m_hideTimer->stop();
	if (m_scoreNote)
		m_scoreNote->hideWorkNote();
	if (left()->isEnabled())
		left()->hide();
	if (right()->isEnabled())
		right()->hide();
	m_scoreNote = 0;
}













