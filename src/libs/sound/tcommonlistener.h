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

#ifndef TCOMMONLISTENER_H
#define TCOMMONLISTENER_H

#include "nootkasoundglobal.h"
#include <music/tnote.h>
#include <music/tnotestruct.h>
#include <QObject>


class TpitchFinder;
class TaudioParams;

/**
 * Common class for managing a pitch detection process.
 * It emits @p noteStarted() when new note was started playing (and its duration is appropriate)
 * and emits @p noteFinished() when note finished.
 * It wraps TpitchFinder values - adjust middle a offset and rejects notes out or range (ambitus)
 *
 * It can be paused - pitch detection is performed but signals are not sending
 * or it can be stopped - incoming audio data is ignored
 * It has pure virtual methods @p startListening(), @p stopListening()
 * those have to be implemented by subclass.
 */
class NOOTKASOUND_EXPORT TcommonListener : public QObject {

  Q_OBJECT

public:
  explicit TcommonListener(TaudioParams* params, QObject* parent = 0);
  ~TcommonListener();


      /** State of input audio device:
       * @p e_listening - when input captures data and emits signals
       * @p e_paused - when data is capturing but signals about detected pitches are not emitting
       * @p e_stopped - capturing data is stopped.
       * Achieved by calling @p stopListening() and @p startListening()
       */
  enum Estate { e_detecting = 0, e_paused = 1, e_stopped = 2 };

        /** Stops emitting signals about pitch detection, but detection is still performed.
       * It also resets last chunk pitch to ignore detection
       * It helps to sniff whole sound/note from begin to its end. */
  void pause() { m_LastChunkPitch = 0.0; if (m_state == e_detecting) setState(e_paused); }

      /** Starts emitting @param noteDetected and @param fundamentalFreq signals again. */
  void unPause() { if (m_state == e_paused) setState(e_detecting); }
  bool isPaused() { return m_state == e_paused; }
  bool isStoped() { return m_state == e_stopped; }

  Estate detectingState() { return m_state; }

  float volume() { return m_volume; } /** Current volume of detecting sound or 0 if silence */

    /** Sets device parameters stores in structure SaudioInParams.
     * SaudioInParams::deviceName is ignored. It have to be set separately. */
  virtual void setAudioInParams();

      /** Sets minimal volume needed that note will be detected. Overrides global setting.  */
  void setMinimalVolume(float minVol);


  void setAmbitus(Tnote loNote, Tnote hiNote);   /** Sets range of notes which are detected. Others are ignored. */
  Tnote& loNote() { return m_loNote; } /** Returns lower boundary note of ambitus */
  Tnote& hiNote() { return m_hiNote; } /** Returns upper boundary note of ambitus */

  qreal lastNotePitch() { return m_lastNote.pitchF; } /** Pitch of last detected note in double precision. */
  TnoteStruct& lastNote() { return m_lastNote; } /** Pitch, frequency and duration of the last detected note. */

  float lastChunkPitch() { return m_LastChunkPitch; }

  bool inRange(qreal pitch) {
    if (pitch >= m_loPitch && pitch <= m_hiPitch) return true;
    else return false;
  }   /** Returns @p TRUE when @p pitch is in ambitus */

  bool noteWasStarted() { return m_noteWasStarted; } /** @p TRUE when note started but not finished. */

      /** Sets pitch detection method. Currently three are available:
       * 0 - MPM (Philip McLeod Method) - default
       * 1 - auto-correlation
       * 2 - MPM modified cepstrum.
       * Currently set value is available through global. */
  void setDetectionMethod(int method);

      /** Stores user action when he stopped sniffing himself. */
  void setStoppedByUser(bool userStop) { m_stoppedByUser = userStop; }
  bool stoppedByUser() { return m_stoppedByUser; }

  quint8 intonationAccuracy(); /** Returns intonation accuracy sets in global audio settings. */
  void setIntonationAccuracy(qint8 intAcc); /** Sets global value of intonation accuracy. It doesn't refresh intonation view. */

  int detectionRange() { return m_currentRange; } /** Integer value of @p TpitchFinder::Erange */


signals:
  void noteStarted(const TnoteStruct&); /** Emitted when note was played and its duration is longer than minimal duration */
  void noteFinished(const TnoteStruct&); /** When already started note fade out */
  void stateChanged(int); /** When device changes its state. It can be cast on @p Estate enumeration. */


public slots:
  virtual void startListening(); /** This virtual method is responsible for starting audio input */
  virtual void stopListening(); /** This virtual method is responsible for stopping audio input */


protected:
  TpitchFinder* finder() { return m_pitchFinder; } /** Instance of @class TpitchFinder */
  void resetVolume() { m_volume = 0.0; } /** Sets volume to 0 */
  void resetChunkPitch() { m_LastChunkPitch = 0.0; }


protected slots:
  void pitchInChunkSlot(float pitch);
  void volumeSlot(float vol) { m_volume = vol; }
  void updateSlot() { setAudioInParams(); }
  void noteStartedSlot(qreal pitch, qreal freq, qreal duration);
  void noteFinishedSlot(TnoteStruct* lastNote);

  void setState(Estate st) { m_state = st; emit stateChanged((int)st); }


private:
  TaudioParams     *m_audioParams;
  TpitchFinder     *m_pitchFinder;
  float             m_volume;
  Tnote             m_loNote, m_hiNote;     /** Boundary notes of the ambitus. */
  TnoteStruct       m_lastNote;
  float             m_LastChunkPitch; /** Pitch from recent processed chunk or 0.0 if silence */
  bool              m_stoppedByUser;
  qreal             m_loPitch, m_hiPitch;
  bool              m_noteWasStarted;
  int               m_currentRange; /** Current range of detected note - see @class TaudioParams */
  Estate            m_state;
};

#endif // TCOMMONLISTENER_H
