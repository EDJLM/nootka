/***************************************************************************
 *   Copyright (C) 2018 by Tomasz Bojczuk                                  *
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


#include "tnotedata.h"

#include <QtCore/qdebug.h>


TnoteData::TnoteData(quint32 fromData)
{
  setData(fromData);
}


/**
 * Upper staff id default state so it is for 0 value
 */
void TnoteData::setOnUpperStaff(bool onUpper) {
  if (onUpper)
    m_otherData &= ~ON_UPPER;
  else
    m_otherData |= ON_UPPER;
}


void TnoteData::setBow(TnoteData::EbowDirection b) {
  m_otherData &= ~BOW_DIRECTION; // reset it first
  m_otherData |= b;
}


void TnoteData::setFinger(int fingerNr) {
  m_otherData &= ~FINGERING;
  if (fingerNr >= -1 && fingerNr < 6)
    m_otherData |= (static_cast<quint16>(fingerNr) + 1) << 3;
  else
    qDebug() << "[TnoteData] wrong finger number to store" << fingerNr << " Igoring.";
}


quint32 TnoteData::data() const {
  quint32 d = m_otherData;
  d <<= 8;
  d += m_fingerPos.data();
  return d;
}


void TnoteData::setData(quint32 d) {
  m_fingerPos.setData(static_cast<quint8>(d));
  m_otherData = static_cast<quint16>(d >> 8);
}


void TnoteData::fromXml(QXmlStreamReader& xml) {
  int s = 0, f = 50;
  while (xml.readNextStartElement()) {
    if (xml.name() == QLatin1String("string"))
      s = xml.readElementText().toInt();
    else if (xml.name() == QLatin1String("fret"))
      f = xml.readElementText().toInt();
    else
      xml.skipCurrentElement();
  }
  if (s == 0 || f == 50)
    m_fingerPos.setData(255); // invalid
  else
    m_fingerPos.setPos(s, f);
}


void TnoteData::toXml(QXmlStreamWriter& xml, const QString& tag) const {
  if (!tag.isEmpty())
    xml.writeStartElement(tag);
  xml.writeTextElement(QLatin1String("string"), QString("%1").arg(m_fingerPos.str()));
  xml.writeTextElement(QLatin1String("fret"), QString("%1").arg(m_fingerPos.fret()));
  if (!tag.isEmpty())
    xml.writeEndElement(); // tag
}
