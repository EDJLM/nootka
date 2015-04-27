/***************************************************************************
 *   Copyright (C) 2013-2015 by Tomasz Bojczuk                             *
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

#include "tabstractsoundview.h"
#include <math.h>


QColor TabstractSoundView::startColor = Qt::green;
QColor TabstractSoundView::middleColor = Qt::yellow;
QColor TabstractSoundView::endColor = Qt::red;
QColor TabstractSoundView::totalColor = QColor(117, 21, 86); // brown
QColor TabstractSoundView::disabledColor = Qt::gray;


TabstractSoundView::TabstractSoundView(QWidget* parent) :
  QWidget(parent)
{
  nootFont = QFont("nootka");
  nootFont.setBold(false);
	tc = palette().color(palette().currentColorGroup(), QPalette::Text);
	disabledColor = palette().color(QPalette::Disabled, QPalette::Text);
	disabledColor.setAlpha(150);
}


void TabstractSoundView::setDisabled(bool isDisabled){
  QWidget::setDisabled(isDisabled);
  if (isDisabled)
    tc = disabledColor;
	else
    tc = palette().color(QPalette::Active, QPalette::Text);
}


    /** Implementation of linear gradient color at given point taken from:
      http://www.qtcentre.org/threads/14307-How-to-get-the-specified-position-s-QColor-in-QLinearGradient */
QColor TabstractSoundView::gradColorAtPoint(float lineX1, float lineX2, QColor startC, QColor endC, float posC) {
  float segmentLength = sqrt((lineX2 - lineX1) * (lineX2 - lineX1));
  double pdist = sqrt((posC - lineX1) * (posC - lineX1));
  double ratio = pdist / segmentLength;
  int red = (int)(ratio * endC.red() + ( 1 - ratio) * startC.red()); //in your case, the values are 12 and 122
  int green = (int)(ratio * endC.green() + (1 - ratio) * startC.green()); //in your case, the values are 23 and 233
  int blue = (int)(ratio * endC.blue() + (1 - ratio) * startC.blue()); //in your case, the values are 24 and 244
  return QColor(red, green, blue);
}


void TabstractSoundView::resizeIt(int myHeight) {
  nootFont.setPointSizeF(20.0);
  QFontMetrics fm(nootFont);
  noteBound = fm.boundingRect("o");
  float factor = (float)myHeight / (float)noteBound.height();
  nootFont.setPointSizeF(20.0 * (((float)myHeight * 0.95) / (float)noteBound.height()));
  noteBound = fm.boundingRect("o");
}


