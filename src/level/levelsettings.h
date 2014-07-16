/***************************************************************************
 *   Copyright (C) 2011-2014 by Tomasz Bojczuk  				                   *
 *   tomaszbojczuk@gmail.com   						                                 *
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


#ifndef LEVELSETTINGS_H
#define LEVELSETTINGS_H

#include <QObject>
#include <QWidget>

class QPushButton;
class TlevelSelector;

/** 
 * This widget contain general level settings and belongs to 
 * @class examsettings dialog.
 */
class levelSettings : public QWidget
{
    Q_OBJECT
public:
    explicit levelSettings(QString &imgPath, QWidget *parent = 0);

    TlevelSelector *levelSelector;
    QPushButton *saveBut, *startExamBut;

};

#endif // LEVELSETTINGS_H
