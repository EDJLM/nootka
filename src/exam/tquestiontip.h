/***************************************************************************
 *   Copyright (C) 2012-2014 by Tomasz Bojczuk                             *
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
 
#ifndef TQUESTIONTIP_H
#define TQUESTIONTIP_H

#include <graphics/tgraphicstexttip.h>
#include <music/tnote.h>
#include <QLinearGradient>

class TfadeAnim;
class Texam;
class Tlevel;
class TQAunit;

/**
 * This is graphics tip (rectangle) representing a question context
 */
class TquestionTip : public TgraphicsTextTip
{
   Q_OBJECT
   
public:
        /** Constructs tip with question content. */
    TquestionTip(Texam *exam, double scale = 0);
    ~TquestionTip();
    
    static QString getTextHowAccid(Tnote::Ealter accid);
    static QString onStringTxt(quint8 strNr); /** Returns translated text on (strNr) string in Nootka font. */
		
				/** Depend on @p instrument it returns text:
				 * 'Play or sing' (other instrument)
				 * 'Play' (guitars) */
		static QString playOrSing(int instr);
		
		static QString& text() { return m_questText; } /** Returns a reference to question HTML string. */
    
    bool freeScore() { return m_scoreFree; } /** true when question is not on score */
    bool freeName() { return m_nameFree; } /** true when question is not on note name */
    bool freeGuitar() { return m_guitarFree; } /** true when question is not on guitar */

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    virtual QRectF boundingRect() const;
		
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    
protected:    
        /** Returns html-formated question text. */
    QString getQuestion(TQAunit &question, int questNr, Tlevel *level, double scale = 0);
    QString getNiceNoteName(Tnote& note, Tnote::EnameStyle style);
        
    
private:
    bool                m_scoreFree, m_nameFree, m_guitarFree; /** Indicate where a tip has to be placed. */
    Tnote::Ealter       m_forcedAccid; /** When different than Tnote::e_Natural text is shown */
    TfadeAnim          *m_fadeInAnim;
		bool								m_markCorner, m_minimized;
		QPointF 						m_lastPos;
		static QString			m_questText;
		QLinearGradient			m_staffGradient;
};


#endif // TQUESTIONTIP_H