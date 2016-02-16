/***************************************************************************
 *   Copyright (C) 2015 by Tomasz Bojczuk                                  *
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

#include "tresulttext.h"
#include "tqaunit.h"
#include "tattempt.h"
#include <QApplication>
#include <QColor>


/** Adds comma and space ', ' to not empty string or returns the same. */
void addSpaceToNotEmpty(QString& txt) {
  if (txt != "")
      txt += ", ";
}


/** Checks the length of string @p txt and adds new line tag if necessary */
void newLineText(QString& txt, const QString& newText) {
  if (txt.length() > 20 && !txt.contains("<br>"))
      txt += "<br>";
  txt += newText;
}


QString wasAnswerOKtext(TQAunit* answer, const QColor& textColor, int fontSize, int attempt) {
  QString txt;
  if (fontSize != -1)
      txt = QString("<span style=\"color: %1; font-size: %2px;\">").arg(textColor.name()).arg(fontSize);
  else
      txt = QString("<span style=\"color: %1;\">").arg(textColor.name());
  TQAunit curQ;
  if (answer->melody() && attempt > 0 && attempt <= answer->attemptsCount())
    curQ.setMistake(answer->attempt(attempt - 1)->summary());
  else
    curQ.setMistake(answer->mistake());
  if (curQ.isCorrect()) {
    txt += QApplication::translate("AnswerText", "Good answer!", "or 'Good!' or 'Correct!' would be somewhat more specific than merely 'It was good!' (previous version) 'It' in this case certainly does refer to a specific thing, which is in this case the answer, but it might be momentarily confused with some other specific thing, such as a shoe or a crocodile, or the wind on one's back. I know that's probably confusing, but the implied subject of 'Correct! is in a certain sense much more specific than a mere 'It' and is more certain to refer to the answer.");
  } else
      if (curQ.wrongNote() || curQ.wrongPos() || curQ.veryPoor())
          txt += QApplication::translate("AnswerText", "Wrong answer!");
      else {
          txt += QApplication::translate("AnswerText", "Not bad, but:", "'Not so bad, but:' is perfectly clear, but a little less common in US English. To be a bit shorter, it might just as well be, 'Not bad, but:'") + "<br>";
          QString misMes = ""; // Message with mistakes
          if (curQ.wrongString())
              misMes = QApplication::translate("AnswerText", "wrong string");
          if (answer->melody() && curQ.littleNotes())
              misMes = QApplication::translate("AnswerText", "little valid notes", "the amount of correct notes in an answer is little");
          if (curQ.poorEffect()) {
              addSpaceToNotEmpty(misMes);
              if (!misMes.isEmpty())
                misMes += "<br>";
              misMes += QApplication::translate("AnswerText", "poor effectiveness");
          }
          if (curQ.wrongAccid())
              misMes = QApplication::translate("AnswerText", "wrong accidental");
          if (curQ.wrongKey()) {
              addSpaceToNotEmpty(misMes);
              newLineText(misMes, QApplication::translate("AnswerText", "wrong key signature"));
          }
          if (curQ.wrongOctave()) {
              addSpaceToNotEmpty(misMes);
              newLineText(misMes, QApplication::translate("AnswerText", "wrong octave"));
          }
          if (curQ.wrongIntonation()) {
              addSpaceToNotEmpty(misMes);
              newLineText(misMes, QApplication::translate("AnswerText", "out of tune"));
          }
          txt += misMes;
      }
  txt += "</span><br>";
  return txt;

}

