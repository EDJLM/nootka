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


#ifndef EXAMSETTINGS_H
#define EXAMSETTINGS_H

#include <QWidget>

class QCheckBox;
class TcolorButton;

class ExamSettings : public QWidget
{
    Q_OBJECT
public:
    explicit ExamSettings(QWidget *parent = 0);

    static QString autoNextQuestTxt() { return tr("ask next question automatically"); }

    void saveSettings();

signals:

public slots:

private:
    QCheckBox *autoNextChB, *repeatIncorChB;
    TcolorButton *questColorBut, *answColorBut;

};

#endif // EXAMSETTINGS_H
