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


#ifndef TRTAUDIOOUT_H
#define TRTAUDIOOUT_H


#include "nootkacoreglobal.h"
#include "tabstractplayer.h"
#include "trtaudio.h"
#include "toggscale.h"
#include <QStringList>

class TaudioParams;

class NOOTKASOUND_EXPORT TaudioOUT : public TabstractPlayer, public TrtAudio
{
  Q_OBJECT
   
public:
    TaudioOUT(TaudioParams *_params, QString &path, QObject *parent = 0);
    virtual ~TaudioOUT();
    
    static QStringList getAudioDevicesList();
    
          /** Starts playing given note and then returns true, otherwise gets false. */
    bool play(int noteNr);
    void setAudioOutParams();
        
        /** Immediately stops playing. Emits nothing */
    void stop();
    
protected:
		static bool outCallBack(void* outBuff, unsigned int nBufferFrames, const RtAudioStreamStatus& status);
		
		int crossCount() { return m_crossCount; } /** counts samples of crossing buffer */
		
protected:
        /** Static pointer of this class instance to emit signal from callBack method. */ 
    static TaudioOUT 							*instance;
    ToggScale 										*oggScale;
    int     				 							 ratioOfRate; // ratio of current sample rate to 44100

private slots:
//   void emitNoteFinished() { emit noteFinished(); }
  void stopSlot();
	void streamOpenedSlot();
	void updateSlot() { setAudioOutParams(); }
    
private:
  static int 			m_samplesCnt; /** Number of performed samples. */
  static int 			m_maxCBloops; /** Duration of a sound counted in callBack loops */
	static qint16  *m_crossBuffer; /** buffer with data of part of previous note to fade out */
	static bool 		m_doCrossFade;
	static float 		m_cross; /** current volume factor of fading effect */
	int 						m_crossCount;
	bool 						m_callBackIsBussy;

};

#endif // TRTAUDIOOUT_H
