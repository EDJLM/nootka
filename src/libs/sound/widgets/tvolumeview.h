/***************************************************************************
 *   Copyright (C) 2013-2014 by Tomasz Bojczuk                             *
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


#ifndef TVOLUMEVIEW_H
#define TVOLUMEVIEW_H

#include "nootkacoreglobal.h"
#include "tabstractsoundview.h"


class NOOTKASOUND_EXPORT TvolumeView : public TabstractSoundView
{

Q_OBJECT

public:
    explicit TvolumeView(QWidget *parent = 0);
    virtual ~TvolumeView();
    
        /** @param alpha is alpha value of background color
        * to manage of the animation of detected pitch */
    void setVolume(float vol, int alpha = 0);
    void setPitchColor (QColor col) { m_pitchColor = col; }
    void setMinimalVolume(float vol) { m_minVolume = vol; update(); }
  
signals:
		void minimalVolume(float);
  
protected:
    virtual void paintEvent(QPaintEvent* );
    virtual void resizeEvent(QResizeEvent*);
		virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent*);
		virtual void enterEvent(QEvent* );
		virtual void leaveEvent(QEvent* );
    
private:
    float           m_volume, m_prevVol;
    QColor          m_pitchColor;
    int             m_alpha; // aplha value of m_pitchColor
    QList<QColor>   m_tickColors;
    int             m_ticksCount;
    int             m_noteWidth;
    float           m_hiTickStep;
    float           m_minVolume; // tick poits minimal vol for pitch detection
    bool 						m_drawKnob, m_leftButton;

};

#endif // TVOLUMEVIEW_H
