/***************************************************************************
 *   Copyright (C) 2017 by Tomasz Bojczuk                                  *
 *   seelook@gmail.com                                                     *
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

#include "tstaffobject.h"
#include "tscoreobject.h"
#include "tmeasureobject.h"
#include "tnoteobject.h"
#include "tnotepair.h"
#include "music/tnote.h"

#include <QtCore/qdebug.h>


TstaffObject::TstaffObject(QObject* parent) :
  QObject(parent),
  m_score(nullptr),
  m_upperLine(16.0),
  m_staffItem(nullptr),
  m_loNotePos(28.0), m_hiNotePos(8.0)
{
}


TstaffObject::~TstaffObject() {
  qDebug() << "[TstaffObject] is going delete";
}


void TstaffObject::setScore(TscoreObject* s) {
  m_score = s;
  m_score->addStaff(this);
}


void TstaffObject::refresh() {
  fit();
  checkNotesRange();
}


void TstaffObject::setUpperLine(qreal upLine) {
  m_upperLine = upLine;
  emit upperLineChanged();
}


void TstaffObject::setStaffItem(QQuickItem* si) {
  m_staffItem = si;
  if (m_score->stavesCount() > 1) { // initial staff position, depends on lowest note in the previous staff
    auto prevStaff = m_score->staff(m_score->stavesCount() - 2);
    m_staffItem->setY(prevStaff->staffItem()->y() + (prevStaff->loNotePos() - hiNotePos()) * prevStaff->scale()); // scale of this staff is not set yet
  }
}


int TstaffObject::firstMeasureNr() {
  return m_measures.empty() ? 0 : m_measures.first()->number();
}


int TstaffObject::number() const {
  return m_staffItem ? m_staffItem->property("number").toInt() : -1;
}


void TstaffObject::setNotesIndent(qreal ni) {
  if (m_notesIndent != ni) {
    m_notesIndent = ni;
    fit();
  }
}


qreal TstaffObject::scale() { return staffItem()->property("scale").toReal(); }



char TstaffObject::debug() {
  QTextStream o(stdout);
  o << "\033[01;34m[" << number() + 1 << " STAFF]\033[01;00m";
  return 32; // fake
}


//#################################################################################################
//###################              PROTECTED           ############################################
//#################################################################################################

#define BARLINE_OFFSET (2.0)
void TstaffObject::fit() {
  if (m_measures.isEmpty() || firstMeasure()->isEmpty()) {
    qDebug() << debug() << "Empty staff - nothing to fit";
    return;
  }

  qreal factor = 2.5;
  qreal gapsSum = 0.0;
  m_allNotesWidth = 0.0;

  for (int m = 0; m < m_measures.size(); ++m) {
    auto measure = m_measures[m];
    m_allNotesWidth += m > 0 ? BARLINE_OFFSET : 0.0; // add bar line space
    for (int n = 0; n < measure->noteCount(); ++n) {
      auto note = measure->note(n)->object();
      gapsSum += note->rhythmFactor();
      m_allNotesWidth += note->width();
      factor = (m_staffItem->width() - m_notesIndent - m_allNotesWidth - 1.0) / gapsSum;
      if (factor < 0.8) { // shift current measure and the next ones
        if (m == 0)
            qDebug() << debug() << "!!!!!! Split this measure among staves !!!!!";
        else {
            for (int nn = n; nn >= 0; --nn) { // revert gapsSum and m_allNotesWidth to state at the end of the previous measure
              auto revertNote = measure->note(nn)->object();
              gapsSum -= revertNote->rhythmFactor();
              m_allNotesWidth -= revertNote->width(); // take bar line space
            }
            if (m > 0)
              m_allNotesWidth -= BARLINE_OFFSET;
            m_gapFactor = (m_staffItem->width() - m_notesIndent - m_allNotesWidth - 1.0) / gapsSum;  // allow factor bigger than 2.5
            m_score->shiftMeasures(measure->number(), m_measures.count() - m);
            updateNotesPos();
            return;
        }
        break; // rest of the notes goes to the next staff
      }
    }
  }

  m_gapFactor = qBound(0.5, factor, 2.5); // notes in this staff are ready to positioning
  updateNotesPos();
}


void TstaffObject::updateNotesPos(int startMeasure) {
  auto firstMeas = m_measures.first();
  if (firstMeas->isEmpty())
    return;

  qDebug() << debug() << "updating notes positions from" << startMeasure << "measure" 
            << "gap factor" << m_gapFactor << "notes count" << lastMeasure()->last()->index() - firstMeasure()->first()->index() + 1;
  TnoteObject* prevNote = nullptr;
  if (startMeasure == 0)
    firstMeas->first()->object()->setX(m_notesIndent);
  else
    prevNote = m_measures[startMeasure - 1]->last()->object();

  for (int m = startMeasure; m < m_measures.size(); ++m) {
    auto measure = m_measures[m];
    qreal barOffset = m > 0 ? BARLINE_OFFSET : 0.0; // offset for first note after bar line
    for (int n = 0; n < measure->noteCount(); ++n) {
      auto note = measure->note(n)->object();
      if (prevNote)
        note->setX(prevNote->rightX() + barOffset);
      prevNote = note;
      barOffset = 0.0;
    }
    measure->checkBarLine();
  }
}


void TstaffObject::checkNotesRange(bool doEmit) {
  qreal oldHi = m_hiNotePos, oldLo = m_loNotePos;
  findHighestNote();
  findLowestNote();
  int nr = number();
  if (nr > 0 && doEmit && oldHi != m_hiNotePos) // do not emit for first staff
    emit hiNotePosChanged(nr, (oldHi - m_hiNotePos) * scale());
  if (doEmit && oldLo != m_loNotePos)
    emit loNotePosChanged(nr, (m_loNotePos - oldLo) * scale());
  return;
}


void TstaffObject::appendMeasure(TmeasureObject* m) {
  m->setStaff(this);
  m_measures << m;
  if (m_measures.count() == 1)
    emit firstMeasureNrChanged();
}


void TstaffObject::takeMeasure(int id) {
  m_measures.removeAt(id);
  if (id == 0)
    emit firstMeasureNrChanged();
}



//#################################################################################################
//###################              PRIVATE             ############################################
//#################################################################################################

void TstaffObject::findHighestNote() {
  m_hiNotePos = upperLine() - 4.0;
  for (int m = 0; m < m_measures.size(); ++m) {
    auto measure = m_measures[m];
    for (int n = 0; n < measure->noteCount(); ++n) {
      auto noteSeg = measure->note(n);
      if (noteSeg->object()->notePosY()) // is visible
        m_hiNotePos = qMin(qreal(noteSeg->object()->notePosY() - (noteSeg->note()->rtm.stemDown() ? 2.0 : 4.0)), m_hiNotePos);
    }
  }
}


void TstaffObject::findLowestNote() {
//   if (hasScordature()) { TODO
//     m_loNotePos = height();
//     return;
//   }
  m_loNotePos =  upperLine() + 13.0; // TODO (isPianoStaff() ? lowerLinePos(): upperLinePos()) + 13.0;
  for (int m = 0; m < m_measures.size(); ++m) {
    auto measure = m_measures[m];
    for (int n = 0; n < measure->noteCount(); ++n) {
      auto noteSeg = measure->note(n);
      m_loNotePos = qMax(qreal(noteSeg->object()->notePosY() + (noteSeg->note()->rtm.stemDown() ? 4 : 2)), m_loNotePos);
    }
  }
}


