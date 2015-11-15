/***************************************************************************
 *   Copyright (C) 2015 by Tomasz Bojczuk                                  *
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

#include "ttouchmessage.h"
#include <animations/tmovedanim.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qdesktopwidget.h>
#include <QtGui/qpalette.h>
#include <QtGui/qpainter.h>
#include <QtCore/qtimer.h>


TtouchMessage::TtouchMessage() :
  TgraphicsTextTip(QString(), qApp->palette().text().color())
{
  setFrameColor(qApp->palette().highlight().color());
  setBaseColor(qApp->palette().highlight().color().lighter());
  setTextWidth(qApp->desktop()->width() * 0.9);

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, this, &TtouchMessage::hideAnim);
  m_anim = new TmovedAnim(this, 0);
  m_anim->setDuration(200);
  hide();
  connect(m_anim, &TmovedAnim::finished, [=] {
            if (pos().y() > qApp->desktop()->height())
              hide();
  });
}


TtouchMessage::~TtouchMessage()
{
  delete m_anim;
}


void TtouchMessage::setMessage(const QString& message, int previewTime) {
  m_timer->stop();
  setHtml(message);
  showAnim();
  if (previewTime > 0)
    m_timer->start(previewTime);
}

//#################################################################################################
//###################              PROTECTED           ############################################
//#################################################################################################

void TtouchMessage::showAnim() {
  m_anim->startMoving(QPointF((qApp->desktop()->width() - realW()) / 2, qApp->desktop()->height() + 8),
                      QPointF((qApp->desktop()->width() - realW()) / 2, qApp->desktop()->height() - realH() - 4));
}


void TtouchMessage::hideAnim() {
  m_timer->stop();
  if (isVisible())
      m_anim->startMoving(pos(), QPointF(pos().x(), qApp->desktop()->height() + 8));
}


void TtouchMessage::mousePressEvent(QGraphicsSceneMouseEvent* event) {
   if (m_anim->isMoving())
      hideAnim();
}








