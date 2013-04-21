/***************************************************************************
 *   Copyright (C) 2011-2013 by Tomasz Bojczuk                             *
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
#ifndef TSOUND_H
#define TSOUND_H

#include <QObject>
#include "tnote.h"

class TabstractPlayer;
class QThread;
class TpitchView;
class TaudioIN;

/** Tsound is a wrapper of TaudioIN & TaudioOUT classes
 * to manage them. It enables/disables them depends on Tglobals,
 * pauses sniffing when playback is proceding.
 * Also it has got TpitchView to show volume meter & pitch detection state.
 */
class Tsound : public QObject
{

  Q_OBJECT
  
public:
  explicit Tsound(QObject *parent = 0);
  virtual ~Tsound();
  
  TabstractPlayer *player;
  TaudioIN  *sniffer;
  
  void play(Tnote note);
  bool isPlayable();
  bool isSniffable() { return (sniffer ? true : false) ; }
    /** Before Nootka config dialog is created a few things have to be done.
     * stop sniffing, playing
     * delete midi, whitch bloks audio devices,
     * delete audioIN, config creates its own to test. */
  void prepareToConf();
    /** Also, when user will discard config, it has to restore its state. */
  void restoreAfterConf();
  void acceptSettings();
  void setPitchView(TpitchView *pView);
    /** Stops sniffing. It is called when en exam is starting. */
  void wait();
    /** Starts sniffing again. */
  void go();
    /** Returns recently detected note. */
  Tnote note() { return m_detNote; }
  void pauseSinffing();
  void unPauseSniffing();
  bool isSnifferPaused();
  void prepareToExam();
  void restoreAfterExam();
    /** Sets bg color to question color and enables TpitchView. */
  void prepareAnswer();
    /** Clears bg color and disables TpitchView. */
  void restoreAfterAnswer();
  void stopPlaying();
  
signals:
  void detectedNote(Tnote note);
  void plaingFinished();
  
private:
  void createPlayer();
  void createSniffer();
  void deletePlayer();
  void deleteSniffer();
  
  TpitchView *m_pitchView;
  Tnote m_detNote; // detected note
  bool m_examMode;
  
private slots:
    /** Is performed when note stops playing, then sniffing is unlocked */
  void playingFinishedSlot();
  void noteDetectedSlot(Tnote note);


};

#endif // TSOUND_H
