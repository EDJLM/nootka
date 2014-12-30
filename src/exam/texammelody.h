/***************************************************************************
 *   Copyright (C) 2014 by Tomasz Bojczuk                                  *
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

#ifndef TEXAMMELODY_H
#define TEXAMMELODY_H

#include <QObject>

class TnoteStruct;


/** 
 * This class manages melodies during exams/exercises.
 * When user plays score, @p newMelody() is called to initialize list of played notes
 * stored in @p listened().
 * Then played note is started, so @p noteStarted() should be invoked to increase an index
 * When note is finished it is stored in the list through @p setNote().
 * When user click a score note to start playing from it @p wasIndexChenged() can be called.
 * then @p wasIndexChenged() returns @p TRUE but invoking @p noteStarted() 
 * will reset it to @p FALSE again.
 * This way can be counted how many times user started to play a melody.
 * 
 * It also manages of a list of fixed melody notes during correction.
 * List is stored in @p attemptFix() and can be reset by @p clearToFix().
 * Then @p setFixed() sets given note to @p TRUE and @p fixed() returns its state.
 */
class TexamMelody : public QObject
{

  Q_OBJECT
  
public:
  
  TexamMelody(QObject* parent);
  ~TexamMelody();
  
  QList<TnoteStruct>& listened() { return m_listened; }
  
  void newMelody(int length); /** Clears list of notes and adds @p length number of empty notes. Sets index to 0. */
  void noteStarted(); /** Increases current index, note is listened. */
  void setNote(const TnoteStruct& n); /** Sets note of current index to given value. */
  
  int currentIndex() { return m_currentIndex; } /** Index of current note in the list. */
  void setCurrentIndex(int id);
  bool wasIndexChanged() { return m_indexChanged; }
  
  QList<bool>& attemptFix() { return m_attemptFix; } /** List of fixed notes in melody during exercise */
  void clearToFix(); /** Clears the list and sets all elements to false. TexamMelody has to be initialized - notes list not empty. */
  bool fixed(int noteNr) { return m_attemptFix[noteNr]; }
  void setFixed(int noteNr) { m_attemptFix[noteNr] = true; } /** Sets given note number to true (fixed) */
  
private:
  
  QList<TnoteStruct>        m_listened;
  int                       m_currentIndex;
  bool                      m_indexChanged;
  
  QList<bool>               m_attemptFix;
};

#endif // TEXAMMELODY_H
