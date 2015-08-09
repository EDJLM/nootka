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


#ifndef TQTAUDIOOUT_H
#define TRTAUDIOOUT_H


#include "nootkasoundglobal.h"
#include "tabstractplayer.h"
#include <QStringList>
#include <QAudio>


class ToggScale;
class TaudioParams;
class QAudioOutput;
class TaudioBuffer;

/** 
 * 
 */
class NOOTKASOUND_EXPORT TaudioOUT : public TabstractPlayer
{
  Q_OBJECT
   
public:
  TaudioOUT(TaudioParams* _params, QObject* parent = 0);
  virtual ~TaudioOUT();

  static QStringList getAudioDevicesList();

        /** Starts playing given note and then returns true, otherwise gets false. */
  bool play(int noteNr);
  void setAudioOutParams();
  void stop(); /** Immediately stops playing. */

  TaudioParams* audioParams() { return m_audioParams; }

protected:
  int crossCount() { return m_crossCount; } /** counts samples of crossing buffer */
  
signals:
  void finishSignal();

protected:
  ToggScale 										*oggScale;
  int     				 							 ratioOfRate; // ratio of current sample rate to 44100

private slots:
  void outCallBack(char* data, qint64 maxLen, qint64& wasRead);
// 	void updateSlot() { setAudioOutParams(); }
	void playingFinishedSlot();
	void stateChangedSlot(QAudio::State state);


private:
  int        			m_samplesCnt; /** Number of performed samples. */
  int 			      m_maxSamples; /** Duration of a sound counted in samples */
  int             m_bufferFrames, m_sampleRate;
	qint16         *m_crossBuffer; /** buffer with data of part of previous note to fade out */
	bool 		        m_doCrossFade;
	float 		      m_cross; /** current volume factor of fading effect */
	int 						m_crossCount;
	bool 						m_callBackIsBussy;
  TaudioParams   *m_audioParams;
  QAudioOutput   *m_audioOUT;
  TaudioBuffer   *m_buffer;
  QString         m_devName;

};

#endif // TRTAUDIOOUT_H
