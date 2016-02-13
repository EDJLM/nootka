/***************************************************************************
 *   Copyright (C) 2011-2016 by Tomasz Bojczuk                             *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <music/tnote.h>
#include <exam/tlevel.h>
#include <QtWidgets/qmainwindow.h>
#include <QtCore/qpointer.h>


class TexamExecutor;
class Tchunk;
class TtoolBar;
class TmainView;
class QPushButton;
class TstatusLabel;
class TmainScore;
class QLabel;
class TfingerBoard;
class TnoteName;
class TmelMan;
class TpitchView;
class Tsound;
class TpluginsLoader;
class TexamView;
class TprogressWidget;
#if !defined (Q_OS_ANDROID)
  class TnootkaLabel;
#endif


/**
 * Implementation of main Nootka window
 */
class MainWindow : public QMainWindow
{
  friend class TexamExecutor;
  friend class Tcanvas;

  Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	TmainView *innerWidget;

public slots:
  void noteWasClicked(int index, Tnote note);
  void guitarWasClicked(const Tnote& note);
  void soundWasStarted(const Tnote& note);
  void soundWasFinished(Tchunk* chunk);


	void openFile(QString runArg); // opens *.nel or *.noo file
	void createSettingsDialog();
	void openLevelCreator(QString levelFile = "");
	void startExamSlot();
	void aboutSlot();
	void analyseSlot();

protected:
  QPointer<TexamExecutor> executor;

	void clearAfterExam(int examState);

	void updateSize(QSize newS); /**< Updates position and sizes of the widgets. */

	void setSingleNoteMode(bool isSingle); /**< Performs changes when insert mode differs then the set one. */
	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *);
#if defined (Q_OS_MAC)
  bool eventFilter(QObject *obj, QEvent *event); /**< Handles opening exam/level files from Finder */
#endif

protected slots:
	void showSupportDialog();
	void updaterMessagesSlot(const QString& m = QString());

      /** This slot is invoked when clef is changed by clicking score.
        * It adjust ambitus to score possibilities if clef is differ than default
        * or to instrument scale if clef backs to default */
  void adjustAmbitus();

private:
	void prepareToExam();

private:
  Tsound               *m_sound;
  TtoolBar             *m_bar; /**< Main Nootka tool bar. */
  TmainScore           *m_score;
  TnoteName            *m_noteName;
  TpitchView           *m_pitchView;
  TfingerBoard         *m_guitar;
  TexamView            *m_examResults;
  TprogressWidget      *m_progress;
	QPixmap 							m_bgPixmap, m_rosettePixmap;
	int 									m_statFontSize;
	bool 									m_levelCreatorExist; /**< Keeps true when Dialog windows is opened, to avoid opening another file. */
	Tlevel 						    m_level;
	bool 									m_isPlayerFree;
	int										m_startedSoundId; /**< Index of note on the score that has been just started.  */

      /** This is tricky workaround when TexamExecutor calls clearAfterExam() where it is deleted
       * and @p executor variable is brought back because execution back to startExamSlot().  */
	bool                  m_deleteExecutor;
  TmelMan							 *m_melButt;

#if !defined (Q_OS_ANDROID)
  TstatusLabel 			   *m_statusLabel;
#endif
  TpluginsLoader       *m_updaterPlugin;
  bool                  m_updaterStoppedSound;
};

#endif // MAINWINDOW_H
