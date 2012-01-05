/***************************************************************************
 *   Copyright (C) 2011 by Tomasz Bojczuk                                  *
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

#ifndef TEXPERTANSWERHELP_H
#define TEXPERTANSWERHELP_H

#include <QDialog>

class QCheckBox;

/** This function creates showExpertAnswersHelpDlg, 
 * and return user decision. */
bool showExpertAnswersHelpDlg(bool &showHelp, QWidget* parent, bool showChBox = true);

/** It shows simple exam help. The parameters are QStrings with colors */
class TexpertAnswerHelp : public QDialog
{
  Q_OBJECT
  
public:
  explicit TexpertAnswerHelp(bool &showHelp, QWidget* parent = 0, bool showChBox = true);
  
protected slots:
  void closeIt();
  
private:
  bool &m_show;
  QCheckBox *showInfoChB;
  
};

#endif // TEXPERTANSWERHELP_H
