/***************************************************************************
 *   Copyright (C) 2011 by Tomasz Bojczuk  				   *
 *   tomaszbojczuk@gmail.com   						   *
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
 *  You should have received a copy of the GNU General Public License	   *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/


#ifndef TPLAYER_H
#define TPLAYER_H

#include "portaudio.h"
#include "RtMidi.h"
#include <QString>

class Tnote;
/** Tplayer is a class which plays single guitar sounds.
  It takes it from file classical-guitar.wav, in method getAudioData checks file
  and load data to m_audioArr. paCallBack method plays it with PortAudio.
  Trick is that data is mono 22050 but output is stereo 44100. All this magic
  is in paCallBack because of requirements of some audio devices.
  So far playing is bounded to range C in Contra octave to e in 3-line.
*/
class Tplayer
{
public:
    Tplayer();
    ~Tplayer();

    static QStringList getAudioDevicesList();

    void play(Tnote note);
        /** It sets device to value taken from Tglobals */
    void setDevice();
    bool isPlayable() { return m_playable; }




private:
        /** Loads wav file with scale to m_audioArr. If everything is ok return true*/
    bool getAudioData();
    static int paCallBack( const void *inBuffer, void *outBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData );
//    int getValueFromChunk(char *chunk, int len);



    PaStreamParameters m_paParam;
    PaError m_paErr;
    PaStream *m_outStream;

    bool m_playable;
//    unsigned short m_chanels;
    quint32 m_sampleRate;
    char *m_audioArr;
        /** maximum number of paCallBack calls per one note (2 sec. of playing) */
    static int m_maxCBloops;
    static int m_samplesCnt;
        /** position of a note in @param m_audioArr */
    static int m_noteOffset;
//########## midi #############
	RtMidiOut *m_midiOut;

};

#endif // TPLAYER_H
