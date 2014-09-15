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


#ifndef TEXAMEXECUTOR_H
#define TEXAMEXECUTOR_H

#include <exam/tqaunit.h>
#include <exam/tlevel.h>
#include <QList>
#include <QColor>

class TequalRand;
class TpitchView;
class Tsound;
class TfingerBoard;
class TnoteName;
class TmainScore;
class Texercises;
class TglobalExamStore;
class Tcanvas;
class QTimer;
class TexecutorSupply;
class Texam;
class MainWindow;
class QAction;


/** 
 * This class manages exam executing and practicing. 
 */
class TexamExecutor : public QObject
{
	friend class MainWindow;

	Q_OBJECT

public:
	explicit TexamExecutor(MainWindow *mainW, QString examFile = "", Tlevel *lev = 0);
	
	~TexamExecutor();
	
	struct TanswerRequire {
			bool octave;
			bool accid;
			bool key;
	};
	
	enum Estate {
		e_starting = 0,
		e_failed, 
		e_openCreator,
		e_questioning,
		e_answering,
		e_saveing,
		e_finished
	}; /** Describes state of exam executing */
	
	bool closeNootka();
	bool isAnswered() { return m_isAnswered; }
	bool isExercise() { return (bool)m_exercise; } /** @p TRUE when exercise or @p FALSE when exam. */

protected:
	MainWindow *mW;
	void deleteExam();

protected slots:
	void askQuestion(bool isAttempt = false);
	
			/** If it is called by pressing "check answer" it obviously shows results
			* but if app is closing it only checks answer and save it without displaying results. */
	void checkAnswer(bool showResults = true);
	void stopExamSlot();
	void stopExerciseSlot();
	void repeatQuestion();
	void repeatSound();
	void playMiddleA();
	void correctAnswer();
	void newAttempt();
	
	void showExamHelp();
	void expertAnswersSlot();
	void startSniffing(); 		/** Invokes Tsound::go() */
	void sniffAfterPlaying(); /** Starts sniffing when asked note is finished */
	void rightButtonSlot();
	void tipButtonSlot(QString name);
	void markAnswer(TQAunit &curQ);
	void delayerTip(); /** This is QTimer slot invoking m_canvas->whatNextTip(true) method. */
	void exerciseToExam(); /** Stops exercising and starts exam. */
	void stopSound(); /** Common method called by exercises and exams to disable sniffing, lock right button, etc. */
	
			/** Performs routines after dialog window closed as such as 
				* right mouse button unlocking. If and exam is going to start it calls @p exerciseToExam() */
	void suggestDialogClosed(bool startExam);
	
			/** Displays message on MainWindow status label about level of started exercise/exam */
	void levelStatusMessage();
	void displayCertificate(); /** Locks the executor and displays certificate. */
	
			/** It sets m_snifferLocked to false (unlocks) and restores capturing right mouse button (installEventFilter) */
	void unlockAnswerCapturing();
	void blindQuestion(); /** Routines for questions with the same answers 'blind' */
	void noteOfMelodySlot(Tnote& n); /** When user plays a melody as an answer. */
	void lastMelodyNote() { checkAnswer(); } /** performed when user played last note of a melody. */
	void prepareToSettings(); /** Should be called when main window is going to display settings dialog. */
	void settingsAccepted(); /** Should be called when settings (Tglobals) was changed during exam. */
	void lockedScoreSlot(int noteNr);

private:
	void createActions();
	void prepareToExam();
	void restoreAfterExam();
			/** Disables score, noteName and guitar*/
	void disableWidgets();
	void clearWidgets();
			/** Clears canvas and invokes restoreAfterExam() */
	void closeExecutor();
	
			/** Performs some initial routines on exam/exercise variables */
	void initializeExecuting();

	QString saveExamToFile();
	void updatePenalStep();
	
			/** Sets texts depend on exercise or exam:
				* - main window title
				* - startExamAct status tip */
	void setTitleAndTexts();
			
private:
	
	TexecutorSupply 					*m_supp; 
	Texam 										*m_exam;
	
				/** main instance of Tlevel, others are pointers or references to it */
	Tlevel 										m_level;
	QList<TQAgroup> 	m_questList;
	
				/** Invokes startSniffing() and stopPlaying() after delay
					* to avoid feedback between played question and listened answer. */
	QTimer 										*m_soundTimer, *m_askingTimer;
	Tnote::EnameStyle 				m_prevQuestStyle, m_prevAnswStyle;
	TglobalExamStore 					*m_glStore;
	TanswerRequire 						m_answRequire;
	
			/** Indicates when sniffing has to be ignored, 
				* because some dialog window exist over exam. */
	bool 											m_snifferLocked;
	bool 											m_shouldBeTerminated, m_isAnswered, m_incorrectRepeated;
	
				/** If it is sets to TRUE locks invoking event of right mouse button.
				* It has to be set before singleShot() method called on askQuestion() 
				* to avoid user click button and call askQuestion() again during time of delay.*/
	bool 											m_lockRightButt;
	bool 											m_goingClosed; /** It becomes true when user wants close Nootka during an exam or exercise.*/
	
				/** stores note if question and answer are Note Name to restore it if question is repeated
				It is to restore buttons state in NoteName widget witch are unchecked by disableWidget() */
	Tnote 										m_prevNoteIfName;
	Tcanvas 									*m_canvas;
	int 											m_blackQuestNr; /** -1 if no black, otherwise points question in blackList list. */
	int 											m_penalStep; /** Interval of questions, after it penalty question is asked */
	int 											m_penalCount; /** Counts questions to ask penalties one. */
	Texercises								*m_exercise;
	int 											m_blindCounter; /** counts occurrences of questions without possible answer */
	TequalRand								*m_rand;
	int												m_melodyNoteIndex;

};

#endif // TEXAMEXECUTOR_H
