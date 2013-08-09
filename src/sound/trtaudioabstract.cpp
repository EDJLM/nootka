/***************************************************************************
 *   Copyright (C) 2013 by Tomasz Bojczuk                                  *
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


#include "trtaudioabstract.h"
#include "taudioparams.h"
#include <QDebug>
#include <QFileInfo>

TrtAudioAbstract::TrtAudioAbstract(TaudioParams* params) :
  deviceName("anything"),
  rtDevice(0),
  audioParams(params),
  streamOptions(0),
  sampleRate(44100)
{
  setUseJACK(params->useJACK);
}

/*static*/
bool TrtAudioAbstract::m_useJACK = true;

RtAudio* TrtAudioAbstract::getRtAudio() {
  RtAudio *rta;
#if defined(Q_OS_LINUX)
    if (m_useJACK)
      rta = new RtAudio(); // check is JACK posible end allow it
    else
      rta = new RtAudio(RtAudio::LINUX_ALSA); // force ALSA
#else
    rta = new RtAudio();
#endif
#if defined(__LINUX_PULSE__)
    if (!m_useJACK /*&& rta->getCurrentApi() != RtAudio::UNIX_JACK*/) { // PulseAudio only when user don't want JACK'
      QFileInfo pulseBin("/usr/bin/pulseaudio");
      if (pulseBin.exists()) {
      RtAudio *rtPulse = new RtAudio(RtAudio::LINUX_PULSE);
        if (rtPulse->getCurrentApi() == RtAudio::LINUX_PULSE) {
          if (rtPulse->getDeviceCount() > 0) {
              delete rta;
              rta = rtPulse;
          }
        }
      }
    }
#endif
    rta->showWarnings(false);
    return rta;
}


void TrtAudioAbstract::printSupportedFormats(RtAudio::DeviceInfo& devInfo) {
  QString fmt;
  if (devInfo.nativeFormats & 0x1)
    fmt += " RTAUDIO_SINT8";
  if (devInfo.nativeFormats & 0x2)
    fmt += " RTAUDIO_SINT16";
  if (devInfo.nativeFormats & 0x4)
    fmt += " RTAUDIO_SINT24";
  if (devInfo.nativeFormats & 0x8)
    fmt += " RTAUDIO_SINT32";
  if (devInfo.nativeFormats & 0x10)
    fmt += " RTAUDIO_FLOAT32";
  if (devInfo.nativeFormats & 0x20)
    fmt += " RTAUDIO_FLOAT32";
  qDebug() << "supported sample formats:" << fmt;
}

void TrtAudioAbstract::printSupportedSampleRates(RtAudio::DeviceInfo& devInfo) {
  QString sRates;
  for (int i = 0; i < devInfo.sampleRates.size(); i++)
    sRates += QString("%1 ").arg(devInfo.sampleRates.at(i));
  qDebug() << "supported sample rates:"<< sRates;
}


bool TrtAudioAbstract::getDeviceInfo(RtAudio::DeviceInfo& devInfo, int id) {
  try {
          devInfo = rtDevice->getDeviceInfo(id);
  }
  catch (RtError& e) {
    qDebug() << "error when probeing input device" << id;
    return false;
  }
  return true;
}



bool TrtAudioAbstract::openStream(RtAudio::StreamParameters* outParams, RtAudio::StreamParameters* inParams,
                                  RtAudioFormat frm, unsigned int rate, unsigned int* buffFrames, RtAudioCallback callBack,
                                  void* userData, RtAudio::StreamOptions* options) {
  try {
    if (rtDevice && !rtDevice->isStreamOpen()) {
//       QString oo = "XX";
//       if (outParams)
//         oo = "out";
//       if (inParams)
//         oo = "in";
//       qDebug() << "openning stream" << oo ;
      rtDevice->openStream(outParams, inParams, frm, rate, buffFrames, callBack, userData, options);
    }
  }
  catch (RtError& e) {
    qDebug() << "can't open stream";
    return false;
  }
  return true;
}


bool TrtAudioAbstract::startStream() {
  try {
    if (rtDevice && !rtDevice->isStreamRunning())
      rtDevice->startStream();
// 		qDebug("stream started");
  }
  catch (RtError& e) {
    qDebug() << "can't start stream";
    return false;
  }
  return true;
}


void TrtAudioAbstract::stopStream() {
  try {
    if (rtDevice && rtDevice->isStreamRunning())
      rtDevice->stopStream();
// 		qDebug("stream stoped");
  }
  catch (RtError& e) {
    qDebug() << "can't stop stream";
  }
}


void TrtAudioAbstract::closeStram() {
  try {
    stopStream();
    if (rtDevice && rtDevice->isStreamOpen())
      rtDevice->closeStream();
// 		qDebug("stream closed");
  }
  catch (RtError& e) {
    qDebug() << "can't close stream";
  }
}


void TrtAudioAbstract::determineSampleRate(RtAudio::DeviceInfo& devInfo) {
  bool rateFound = false;
  for (int i = 0; i < devInfo.sampleRates.size(); i++) {
    unsigned int &sr = devInfo.sampleRates.at(i);
    if ( sr == 44100 || sr == 48000 || sr == 88200 || sr == 96000 || sr == 176400 || sr == 192000) {
      rateFound = true;
      break;
    }
  }
  if (!rateFound)
    sampleRate = devInfo.sampleRates.at(devInfo.sampleRates.size() - 1);
}









