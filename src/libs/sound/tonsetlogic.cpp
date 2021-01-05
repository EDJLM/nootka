/***************************************************************************
 *   Copyright (C) 2021 by Tomasz Bojczuk                                  *
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

#include "tonsetlogic.h"
#include "tartini/notedata.h"

#include "QtCore/qdebug.h"


TonSetChunk::TonSetChunk(float* d, int len)
{
  setData(d, len);
}


TonSetChunk::TonSetChunk(float vol) {
  m_hiVol = vol;
}

void TonSetChunk::setData(float* d, int len) {
  float prevSample = 1.0, sample = 0.0;
  float tmpMaxVol = 0.0;
  bool isPositive = false;
  for (int i = 0; i < len; ++i) {
    sample = d[i];
    if (i > 0) {
      // look up for max peak value only in positive part of an audio signal
      if ((prevSample < 0.0f || !prevSample) && (!sample || sample > 0.0f)) { // positive part
          isPositive = true;
      } else if ((!prevSample || prevSample > 0.0) && (sample < 0.0f || !sample)) { // negative part
          m_hiVol = qMax(m_hiVol, tmpMaxVol);
          isPositive = false;
          tmpMaxVol = 0.0;
      }
      if (isPositive) {
        tmpMaxVol = qMax(tmpMaxVol, sample);
      }
    }
    prevSample = sample;
  }
}

//#################################################################################################
//###################           TonSetLogic class      ############################################
//#################################################################################################

#define CHUNKS_TO_LOOK (3)


TonSetLogic::TonSetLogic() :
  m_minDuration(CHUNKS_TO_LOOK),
  m_totalMaxVol(0.25f)
{}


TonSetLogic::~TonSetLogic() {}


void TonSetLogic::reset() {
  m_chunkNr = 0;
  m_startedAt = 0;
  m_finishedAt = 0;
  m_totalMaxVol = 0.2f;
  m_noteVolume = e_volInitial;
  m_noteStarted = false;
  m_noteWasStarted = false;
  m_restWasStarted = false;
  m_chunks.clear();
}


void TonSetLogic::analyzeChunk(float *d, int len) {
  if (m_chunks.size() >= CHUNKS_TO_LOOK)
    m_chunks.removeFirst();
  m_chunks << TonSetChunk(d, len);
  m_totalMaxVol = qMax(m_totalMaxVol, m_chunks.last().hiVol());

  float hiVol = 0.0, loVol = 1.0;
  int hiAt, loAt;
  for (int c = 0; c < m_chunks.size(); ++c) {
    if (m_chunks[c].hiVol() > hiVol) {
      hiAt = c;
      hiVol = m_chunks[c].hiVol();
    }
    if (m_chunks[c].hiVol() < loVol) {
      loAt = c;
      loVol = m_chunks[c].hiVol();
    }
  }

  if (hiAt > loAt)
    m_dynamicVal = hiVol - loVol;
  else
    m_dynamicVal = 0.0;

  m_noteFinished = false;
  m_restFinished = false;

  // silence threshold is 1/25 of max PCM volume occurred
  if (m_chunks.last().hiVol() < m_totalMaxVol / 25.0) {
      if (m_noteVolume == e_volPending) {
          m_noteVolume = e_volSilence;
          m_silenceAt = m_chunkNr;
      } else if (m_noteVolume == e_volSilence) {
          if (m_chunkNr - m_silenceAt == m_minDuration) {
            if (m_noteWasStarted) {
              m_finishedAt = m_silenceAt - 1;
              m_noteFinished = true;
              m_noteWasStarted = false;
            }
          }
      } else if (m_noteVolume == e_volInitial)
          {} // do nothing
      else
          qDebug() << "[TonSetLogic] Some unexpected volume state occurred - investigate it!" << m_noteVolume;
  } else {
      // check dynamic of volume growth - it has to be 1/4 of max PCM volume
      if (m_noteVolume != e_volOnSet && m_dynamicVal > m_totalMaxVol * 0.25f // volume grew enough
          && m_chunkNr - m_startedAt > m_minDuration) // last note was long enough
      {
          if (m_noteWasStarted) {
            m_finishedAt = m_chunkNr - (CHUNKS_TO_LOOK - loAt);
            m_noteFinished = true;
            m_noteWasStarted = false;
          }
          if (m_restWasStarted) {
            m_restWasStarted = false;
            m_restFinished = true;
          }
          m_startedAt = m_chunkNr - (CHUNKS_TO_LOOK -1 - loAt); // note starts one chunk after the latest chunk with lowest volume
          m_noteVolume = e_volOnSet;
      } else if (m_noteVolume == e_volOnSet)
          m_noteVolume = e_volPending;
  }

  m_noteStarted = m_noteVolume == e_volPending && m_chunkNr - m_startedAt == m_minDuration;
  if (m_noteStarted)
    m_noteWasStarted = true;
  m_restStarted = m_noteVolume == e_volSilence && m_chunkNr - m_silenceAt == m_minDuration;
  if (m_restStarted)
    m_restWasStarted = true;

  m_chunkNr++;
}

