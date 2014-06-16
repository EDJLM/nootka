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

#ifndef TSCOREPARAMS_H
#define TSCOREPARAMS_H

#include "nootkacoreglobal.h"
#include "music/tnote.h"
#include "music/tclef.h"
#include <QColor>


/** 
 * This is set of parameters related to score and note names
 */
class NOOTKACORE_EXPORT TscoreParams
{
public:
	
//============ score widget settings =============================================================
        /** if true shows other similar (enharmonic) notes on the staff:
        * for C# - Db, for G - Fx and Abb. In Tnotename widget also. */
    bool keySignatureEnabled;
    bool showKeySignName; // default true
    Tnote::EnameStyle nameStyleInKeySign;
		
        /** Convention is:
        * if keyNameSuffix == " " constructor of Tglobals sets its default and
        * constructor of TkeySignatureView sets translatable value "major" and "minor"
        * otherwise it is overridden by loading settings
        * if keyNameSuffix == "" user prefers without suffix.
        * If keyNameSuffix has some text -  is translated by user himself */
    QString majKeyNameSufix;
    QString minKeyNameSufix;
    QColor pointerColor;
		Tclef::Etype clef; // preferred clef - treble by default
		bool isSingleNoteMode; // score mode (single or multi)
		qreal scoreScale; // score scale - user preferred staff size
    int tempo; // playback tempo

//============= common with score widget and note name ==========================================
    bool doubleAccidentalsEnabled; //default true
    bool showEnharmNotes; // default true
    
        /** On the very beginning it is -1 and then it is set in TscoreWidget constructor
        * as inversion of highlight color from palette() and put to TnoteName,
        * otherwise is taken from saved settings. */
    QColor enharmNotesColor;
    bool seventhIs_B; /** To determine note names - default true */


//======== note name settings ===================================================================
    Tnote::EnameStyle nameStyleInNoteName;
    bool octaveInNoteNameFormat; //default true
    Tnote::EnameStyle solfegeStyle; // e_italiano_Si is default
    bool namesOnScore; // show/hide note names on the score
    QColor nameColor;// color of note name highlight

};

#endif // TSCOREPARAMS_H
