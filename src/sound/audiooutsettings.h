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


#ifndef AUDIOOUTSETTINGS_H
#define AUDIOOUTSETTINGS_H

#include <QWidget>

class TaudioParams;
class QComboBox;
class QGroupBox;
class QRadioButton;

class AudioOutSettings : public QWidget
{
    Q_OBJECT
public:
    explicit AudioOutSettings(TaudioParams *aParams, QWidget *parent = 0);

    void saveSettings();
		/** The structure describes midi program (instrument).
		 * It has @param name 
		 * and @param progNr which is midi instrument number  */
	struct TmidiInstrListStruct {
		QString name;
		unsigned char progNr;
	};
		/** A list of midi instruments used in Nootka */
	QList<TmidiInstrListStruct> instruments;
  
protected:
virtual void focusInEvent(QFocusEvent* );


private:
	void addInstrument(QString name, unsigned char midiNr);

  QGroupBox *audioOutEnableGr, *realAGr, *midiGr;
  QComboBox *audioOutDevListCombo, *midiPortsCombo, *midiInstrCombo;
  QRadioButton *midiRadioButt, *audioRadioButt;
  TaudioParams *m_params;
    /** Is true when widget is shown first time. 
     * Then devices lists are taken. */
  bool m_firstTime;
	
private slots:
	void audioOrMidiChanged();

};

#endif // AUDIOOUTSETTINGS_H
