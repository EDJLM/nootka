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

#include "trandmelody.h"
#include <music/tmelody.h>
#include <music/trhythm.h>



void getRandomMelody(QList<TQAunit::TQAgroup>& qList, Tmelody* mel, int len, bool inKey, bool onTonic) {
	for (int i = 0; i < len; ++i) {
		int randVal = qrand() % qList.size();
		Tnote pitch = qList[randVal].note;
		TfingerPos fPos = qList[randVal].pos;
		if (inKey) { // note has to be in key signature declared in melody
			if (mel->key().inKey(pitch).note == 0) { // if it is not
				int tryCount = 0;
				int it = randVal;
				while (tryCount < qList.size()) { // find another one
					it++;
					if (it == qList.size())
						it = 0;
					Tnote tryNote = mel->key().inKey(qList[it].note);
					if (tryNote.note != 0) {
						pitch = tryNote;
						fPos = qList[it].pos;
						break;
					}
					tryCount++;
				}
			}
		}
		Tchunk note(pitch, Trhythm(), fPos);
		mel->notes().append(note);
	}
	if (onTonic) {
		char tonicNoteNr;
		if (mel->key().isMinor())
			tonicNoteNr = TkeySignature::minorKeys[mel->key().value() + 7];
		else
			tonicNoteNr = TkeySignature::majorKeys[mel->key().value() + 7];
		for (int i = 0; i < qList.size(); ++i) {
			Tnote tonic(tonicNoteNr + 1, 0, TkeySignature::scalesDefArr[mel->key().value() + 7][tonicNoteNr]);
			bool theSame = false;
			if (tonic.compareNotes(qList[i].note, true))
				theSame = true;
			else {
				Tnote tonicConverted;
				if (tonic.alter == 1)
					tonicConverted = tonic.showWithFlat();
				else if (tonic.alter == -1)
					tonicConverted = tonic.showWithSharp();
				else
					continue;
				if (tonicConverted.compareNotes(qList[i].note, true))
					theSame = true;
			}
			if (theSame) {
				tonic.octave = qList[i].note.octave;
				mel->notes().last().p() = tonic;
				mel->notes().last().g() = qList[i].pos;
				break;
			}
		}
	}
}

