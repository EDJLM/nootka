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


#ifndef TCLEF_H
#define TCLEF_H

// #include <QDataStream>


class Tclef
{
  
public:
    enum EclefType {
      e_treble_G = 1, // common treble clef
      e_bass_F = 2, // bass clef
      e_alt_C = 4,
      e_treble_G_8down = 8, // treble clef with "8" digit below (guitar)
      e_bass_F_8down // bass clef with "8" digit below (bass guitar)
    }
  
    Tclef(EclefType type = e_treble_G);
    
//     friend QDataStream &operator<< (QDataStream &out, const TfingerPos &fPos) {
//         out << fPos.m_pos;
//         return out;
//     }
//     friend QDataStream &operator>> (QDataStream &in, TfingerPos &fPos) {
//         in >> fPos.m_pos;
//         return in;
//     }

private:
    EclefType m_type;
    
};

Tclef::EclefType Tclef::Tclef(Tclef::EclefType type) {
  m_type = type;
}


#endif // TCLEF_H
