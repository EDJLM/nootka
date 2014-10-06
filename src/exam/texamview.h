/***************************************************************************
 *   Copyright (C) 2011-2014 by Tomasz Bojczuk                             *
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


#ifndef TEXAMVIEW_H
#define TEXAMVIEW_H

#include <QWidget>
#include <QTime>
#include <QTimer>

class Texam;
class TQAunit;
class QLabel;


/** 
 * A @class TexamView represents status of exam.
 * It displays times and numbers of valid/invalid questions.
 * @p startExam(Texam) Initializes it.
 * @p questionStart() starts timer for a new question
 * @p questionStop() stops timer and writes elapsed time to @p Texam
 * @p answered() summarizes and displays counters numbers on labels
 */
class TexamView : public QWidget
{
    Q_OBJECT
public:
    explicit TexamView(QWidget *parent = 0);
		
      /** Returns time given in milisec. in format h:mm:ss */
    static QString formatedTotalTime(int t) { return QString("%1:%2:%3")
            .arg(t/3600000)
            .arg((t%3600000)/60000, 2, 'f', 0, '0')
            .arg((t%60000)/1000, 2, 'f', 0, '0'); }

		void startExam(Texam *exam); /** Initialization */		
		void questionStart(); /** Starts counting time for a new question */
		void questionStop(); /** Stops question timer and updates time in the last exam TQAunit. */
		void answered(); /** Displays updated counters after answer */
		
    quint16 questionTime(); /** Elapsed time for of current question [seconds * 10] */
    void setFontSize(int s);
		
		void pause(); /** Stops counting time of answer */
		void go(); /** Continues counting time of answer */

    void stopExam() { m_timer->stop(); }
    
    void doNotdisplayTime() { m_timer->stop(); } /** Stops displaying pending time */
    void displayTime() { 	m_timer->start(100); } /** Starts Refreshing elapsing time every 100 ms  */
    
    quint32 totalTime() {return m_startExamTime + quint32(m_totalTime.elapsed() / 1000); } /** Total time of the exam. */
    void updateExam(); /** Updates elapsed time and average time in current @p Texam */
    void clearResults();
		
        /** Sets background of mistakes/correct answers number Qlabel.
         * Background color is directly inserted to setStyleSheet so 
         * it has to be given in format: 
         * background-color: rgba(red, green, blue, alpha) */
    void setStyleBg(QString okBg, QString wrongBg, QString notBadBg = "");

    virtual QSize sizeHint() const { return m_sizeHint; }

public slots:
		void reactTimesUpdate(); /** Updates reaction time and average reaction time of to the last answer time. */
		void effectUpdate(); /** Updates effectiveness value of an exam. */
		void questionCountUpdate(); /** Updates mistakes, correct, and 'not bad' numbers to current exam values */
    
private:
    bool 			m_showReact; // switches whether displays pending question time counter
    QLabel 	 *m_reactTimeLab, *m_averTimeLab, *m_totalTimeLab;
    QLabel 	 *m_mistLab, *m_corrLab, *m_effLab, *m_halfLab;
		QTime 		m_questionTime; // Elapsing time of a question - started with questionStart() and stopped with questionStop()
		int				m_startExamTime; // Elapsed time from previous exam sessions
    QTimer 	 *m_timer;
    QTime 		m_totalTime; // Total time of an exam
		int 			m_pausedAt; // when m_averTime was paused
		QSize			m_sizeHint;
		Texam		 *m_exam;

private slots:
    void countTime();

};

#endif // TEXAMVIEW_H
