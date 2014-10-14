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


#include "questionssettings.h"
#include "tkeysigncombobox.h"
#include <widgets/tquestionaswdg.h>
#include <exam/tlevel.h>
#include <tnoofont.h>
#include <widgets/tintonationview.h>
#include <QtWidgets>


QString tableTip(const QString& tipText, TQAtype::Etype qType, TQAtype::Etype aType, int fSize) {
	return "<table valign=\"middle\" align=\"center\"><tr><td>" + 
	TnooFont::span(TquestionAsWdg::qaTypeSymbol(qType) + "?", fSize) + "</td>" + "<td align=\"center\">" + tipText + " </td> " +
	"<td>" + TnooFont::span(TquestionAsWdg::qaTypeSymbol(aType) + "!", fSize) + "</td></tr></table>";
}

questionsSettings::questionsSettings(TlevelCreatorDlg* creator) :
    TabstractLevelPage(creator)
{
	QVBoxLayout *mainLay = new QVBoxLayout;
	mainLay->addStretch();

	int nootFontSize = fontMetrics().boundingRect("A").height() * 2;
	QString nooColor = QString("color: %1").arg(palette().highlight().color().name());
	m_tableWdg = new QWidget(this);
	QHBoxLayout *tabLay = new QHBoxLayout;
	tabLay->addWidget(m_tableWdg);
	QGridLayout *qaLay = new QGridLayout(); // Questions & Answers table
	qaLay->setAlignment(Qt::AlignCenter);
	qaLay->setSpacing(10);
// Labels describing answers types
	QFont f = font();
	f.setBold(true);
	QLabel *newQuestLab = new QLabel(TquestionAsWdg::answerTxt().toUpper(), this);
	newQuestLab->setFont(f);
	qaLay->addWidget(newQuestLab, 0, 2, 0, 4, Qt::AlignHCenter | Qt::AlignTop);
	m_questLab = new QLabel(TnooFont::span("n", nootFontSize * 1.5, nooColor) + "<br><br>" + TquestionAsWdg::questionTxt().toUpper(), this);
	m_questLab->setAlignment(Qt::AlignCenter);
	m_questLab->setFont(f);
	qaLay->addWidget(m_questLab, 1, 0, Qt::AlignBottom | Qt::AlignHCenter);
	m_answLab = new QLabel("  ", this);
	m_answLab->setFont(f);
	qaLay->addWidget(m_answLab, 1, 1, Qt::AlignBottom);
	QLabel *asNoteLab = new QLabel(" <br>" + TquestionAsWdg::asNoteTxt().replace(" ", "<br>"), this);
	asNoteLab->setAlignment(Qt::AlignCenter);
	qaLay->addWidget(asNoteLab, 1, 2, Qt::AlignBottom);
	QLabel *asNameLab = new QLabel(" <br>" + TquestionAsWdg::asNameTxt().replace(" ", "<br>"), this);
	asNameLab->setAlignment(Qt::AlignCenter);
	qaLay->addWidget(asNameLab, 1, 3, Qt::AlignBottom);
	m_asFretLab = new QLabel(" <br>" + TquestionAsWdg::asFretPosTxt().replace(" ", "<br>"), this);
	m_asFretLab->setAlignment(Qt::AlignCenter);
	qaLay->addWidget(m_asFretLab, 1, 4, Qt::AlignBottom);
	m_asSoundLab = new QLabel(" <br>" + TquestionAsWdg::asSoundTxt().replace(" ", "<br>"), this);
	m_asSoundLab->setAlignment(Qt::AlignCenter);
	qaLay->addWidget(m_asSoundLab, 1, 5, Qt::AlignBottom);
// CheckBoxes with types of answers for every kind of question
	asNoteWdg = new TquestionAsWdg(TQAtype::e_asNote, qaLay, 2, this);
	asNameWdg = new TquestionAsWdg(TQAtype::e_asName, qaLay, 3, this);
	asFretPosWdg = new TquestionAsWdg(TQAtype::e_asFretPos, qaLay, 4, this);
	asSoundWdg = new TquestionAsWdg(TQAtype::e_asSound, qaLay, 5, this);
// Labels on the right side of the table with symbols of types - related to questions
	QLabel *scoreNooLab = new QLabel("s?", this);
	QFont nf("nootka", fontMetrics().boundingRect("A").height());
#if defined(Q_OS_MACX)
	nf.setPointSize(fontMetrics().boundingRect("A").height() * 2);
#endif
	scoreNooLab->setFont(nf);
	qaLay->addWidget(scoreNooLab, 2, 6, Qt::AlignCenter);
	QLabel *nameNooLab = new QLabel("c?", this);
	nameNooLab->setFont(nf);
	qaLay->addWidget(nameNooLab, 3, 6, Qt::AlignCenter);
	m_guitarNooLab = new QLabel("g?", this);
	m_guitarNooLab->setFont(nf);
	qaLay->addWidget(m_guitarNooLab, 4, 6, Qt::AlignCenter);
	m_soundNooLab = new QLabel("n?", this);
	m_soundNooLab->setFont(nf);
	qaLay->addWidget(m_soundNooLab, 5, 6);
// Labels on the bottom side of the table with symbols of types - related to answers  
	QLabel *qScoreNooLab = new QLabel("s!", this);
	qScoreNooLab->setFont(nf);
	qaLay->addWidget(qScoreNooLab, 6, 2, Qt::AlignCenter);
	QLabel *qNmeNooLab = new QLabel("c!", this);
	qNmeNooLab->setFont(nf);
	qaLay->addWidget(qNmeNooLab, 6, 3, Qt::AlignCenter);
	m_qGuitarNooLab = new QLabel("g!", this);
	m_qGuitarNooLab->setFont(nf);
	qaLay->addWidget(m_qGuitarNooLab, 6, 4, Qt::AlignCenter);
	m_qSoundNooLab = new QLabel("n!", this);
	m_qSoundNooLab->setFont(nf);
	qaLay->addWidget(m_qSoundNooLab, 6, 5);
	
	QLabel *melodyLab = new QLabel(TnooFont::span("m", nootFontSize * 2, nooColor), this);
	melodyLab->setAlignment(Qt::AlignCenter);
	m_playMelodyChB = new QCheckBox(tr("play melody"), this);
		m_playMelodyChB->setStatusTip(tableTip(tr("Play a melody written in a score"), 
																					 TQAtype::e_asNote, TQAtype::e_asSound, nootFontSize));
	m_writeMelodyChB = new QCheckBox(tr("write melody"), this);
		m_writeMelodyChB->setStatusTip(tableTip(tr("Listen to a melody and write it on a score"), 
																						TQAtype::e_asSound, TQAtype::e_asNote, nootFontSize));
// 		QCheckBox *m_repeatMelodyChB = new QCheckBox(tr("repeat melody"), this);
	m_melodyLengthSpin = new QSpinBox(this);
		m_melodyLengthSpin->setMaximum(100);
		m_melodyLengthSpin->setMinimum(1);
		m_melodyLengthSpin->setMaximum(50);
		m_melodyLengthSpin->setStatusTip(tr("Maximum number of notes in a melody."));
	QLabel *lenghtLab = new QLabel(tr("Melody length"), this);
		lenghtLab->setStatusTip(m_melodyLengthSpin->statusTip());
		
	m_finishOnTonicChB = new QCheckBox(tr("Melody ends on tonic note"), this);
		m_finishOnTonicChB->setStatusTip(tr("Determines the last note of a melody.<br>When set, melody will be finished on tonic note in actual key signature."));
	
	m_tableWdg->setLayout(qaLay);
	m_singleGr = new QGroupBox(tr("single note"), this);
			m_singleGr->setCheckable(true);
			m_singleGr->setLayout(tabLay);
	
	QVBoxLayout *melLay = new QVBoxLayout;
		melLay->addWidget(melodyLab, 0, Qt::AlignCenter);
		melLay->addStretch();
		melLay->addWidget(m_playMelodyChB);
		melLay->addWidget(m_writeMelodyChB);
// 			melLay->addWidget(m_repeatMelodyChB);
		melLay->addStretch();
		QHBoxLayout *lenLay= new QHBoxLayout;
			lenLay->addWidget(lenghtLab);
			lenLay->addWidget(m_melodyLengthSpin);
		melLay->addLayout(lenLay);
		melLay->addWidget(m_finishOnTonicChB);
		melLay->addStretch();
	m_melodiesGr = new QGroupBox(tr("melodies"), this);
			m_melodiesGr->setCheckable(true);
			m_melodiesGr->setLayout(melLay);
	QHBoxLayout *grBoxLay = new QHBoxLayout;
		grBoxLay->addStretch();
		grBoxLay->addWidget(m_singleGr);
		grBoxLay->addStretch();
		grBoxLay->addWidget(m_melodiesGr);
		grBoxLay->addStretch();
	
	mainLay->addLayout(grBoxLay);
	mainLay->addStretch();
	
// some checkBoxes
	QGridLayout *chLay = new QGridLayout;
	octaveRequiredChB = new QCheckBox(tr("require octave"),this);
	octaveRequiredChB->setStatusTip(tr("if checked, selecting of valid octave is required"));
	chLay->addWidget(octaveRequiredChB, 0, 0, Qt::AlignLeft);
	
	styleRequiredChB = new QCheckBox(tr("use different naming styles"),this);
	styleRequiredChB->setStatusTip(tr("if checked, note names are switched between letters and solfege."));
	chLay->addWidget(styleRequiredChB, 1, 0, Qt::AlignLeft);
	
	showStrNrChB = new QCheckBox(tr("show string number in questions"), this);
	showStrNrChB->setStatusTip(tr("Shows on which string an answer has to be given.<br>Be careful, sometimes it is needed and sometimes it makes no sense."));
	chLay->addWidget(showStrNrChB, 0, 1, Qt::AlignLeft);
	mainLay->addLayout(chLay);
	
	lowPosOnlyChBox = new QCheckBox(tr("notes in the lowest position only"),this);
	lowPosOnlyChBox->setStatusTip(tr("if checked, the lowest position in selected fret range is required,<br>otherwise all possible positions of the note are acceptable.<br>To use this, all strings have to be available!"));
	chLay->addWidget(lowPosOnlyChBox, 1, 1, Qt::AlignLeft);
	
	TintonationCombo *intoCombo = new TintonationCombo(this);
	m_intonationCombo = intoCombo->accuracyCombo; // we need only combo box (label is not necessary)
	mainLay->addWidget(intoCombo, 0, Qt::AlignCenter);
	
		
	setLayout(mainLay);

	connect(asNoteWdg, SIGNAL(answerStateChanged()), this, SLOT(whenParamsChanged()));
	connect(asNameWdg, SIGNAL(answerStateChanged()), this, SLOT(whenParamsChanged()));
	connect(asFretPosWdg, SIGNAL(answerStateChanged()), this, SLOT(whenParamsChanged()));
	connect(asSoundWdg, SIGNAL(answerStateChanged()), this, SLOT(whenParamsChanged()));
	
	connect(octaveRequiredChB , SIGNAL(clicked()), this, SLOT(whenParamsChanged()));
	connect(styleRequiredChB, SIGNAL(clicked()), this, SLOT(whenParamsChanged()));
	connect(showStrNrChB, SIGNAL(clicked()), this, SLOT(whenParamsChanged()));
	connect(lowPosOnlyChBox, SIGNAL(clicked()), this, SLOT(whenParamsChanged()));
	connect(m_intonationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(whenParamsChanged()));
	connect(m_playMelodyChB, SIGNAL(clicked()), this, SLOT(whenParamsChanged()));
	connect(m_writeMelodyChB, SIGNAL(clicked()), this, SLOT(whenParamsChanged()));
	connect(m_melodyLengthSpin, SIGNAL(valueChanged(int)), this, SLOT(whenParamsChanged()));
	
	connect(m_singleGr, SIGNAL(clicked()), this, SLOT(singleMultiSlot()));
	connect(m_melodiesGr, SIGNAL(clicked()), this, SLOT(singleMultiSlot()));
	connect(m_playMelodyChB, SIGNAL(clicked()), this, SLOT(melodyQuestionSlot()));
	connect(m_writeMelodyChB, SIGNAL(clicked()), this, SLOT(melodyQuestionSlot()));
	
	m_singleGr->setChecked(false);
}


void questionsSettings::loadLevel(Tlevel* level) {
	blockSignals(true);
    asNoteWdg->setAnswers(level->answersAs[TQAtype::e_asNote]);
    asNoteWdg->setChecked(level->questionAs.isNote()); // when it is false it cleans all checkBoxes to false
    asNameWdg->setAnswers(level->answersAs[TQAtype::e_asName]);
    asNameWdg->setChecked(level->questionAs.isName());
    asFretPosWdg->setAnswers(level->answersAs[TQAtype::e_asFretPos]);
    asFretPosWdg->setChecked(level->questionAs.isFret());
    asSoundWdg->setAnswers(level->answersAs[TQAtype::e_asSound]);
    asSoundWdg->setChecked(level->questionAs.isSound());
    
    octaveRequiredChB->setChecked(level->requireOctave);
    styleRequiredChB->setChecked(level->requireStyle);
    showStrNrChB->setChecked(level->showStrNr);
    lowPosOnlyChBox->setChecked(level->onlyLowPos);
		m_intonationCombo->setCurrentIndex(level->intonation);
		if (level->melodyLen == 1)
			m_melodyLengthSpin->setRange(1, 1);
		else
			m_melodyLengthSpin->setRange(2, 50);
		m_melodyLengthSpin->setValue(level->melodyLen);
		m_finishOnTonicChB->setChecked(level->endsOnTonic);
		saveLevel(wLevel());
		if (wLevel()->canBeMelody()) {
			m_melodiesGr->setChecked(true);
			m_singleGr->setChecked(false);
			if (level->questionAs.isNote() && level->answersAs[TQAtype::e_asNote].isSound())
				m_playMelodyChB->setChecked(true);
			else
				m_playMelodyChB->setChecked(false);
			if (level->questionAs.isSound() && level->answersAs[TQAtype::e_asSound].isNote())
				m_writeMelodyChB->setChecked(true);
			else
				m_writeMelodyChB->setChecked(false);
		} else {
				m_singleGr->setChecked(true);
				m_melodiesGr->setChecked(false);
				m_playMelodyChB->setChecked(false);
				m_writeMelodyChB->setChecked(false);
		}
	blockSignals(false);
}


void questionsSettings::changed() {

}


void questionsSettings::whenParamsChanged() {	
	// Disable 'show string' and 'lowest position only' when no answers as guitar and/or sound
		if (lowPosOnlyChBox->isVisible()) {
			bool lowDisabled = false;
			if (!asNoteWdg->answerAsPos() && !asNameWdg->answerAsPos() && 
								!asFretPosWdg->answerAsPos() && !asSoundWdg->answerAsPos() &&
								!asNoteWdg->answerAsSound() && !asNameWdg->answerAsSound() && 
								!asFretPosWdg->answerAsSound() && !asSoundWdg->answerAsSound())
					lowDisabled = true;		
  // Disable showStrNrChB & lowPosOnlyChBox  if question and answer are on guitar
			if (asFretPosWdg->isChecked() && asFretPosWdg->answerAsPos()) {
				showStrNrChB->setChecked(true);
				lowPosOnlyChBox->setChecked(false);
				lowDisabled = true;
			}
			lowPosOnlyChBox->setDisabled(lowDisabled);
			showStrNrChB->setDisabled(lowDisabled);
		}
	// Is sound input enabled to enable intonation
		if (asNoteWdg->answerAsSound() || asNameWdg->answerAsSound() || asFretPosWdg->answerAsSound() || asSoundWdg->answerAsSound())
				m_intonationCombo->setDisabled(false);
		else
				m_intonationCombo->setDisabled(true);
    changedLocal();
}


void questionsSettings::hideGuitarRelated() {
		asFretPosWdg->enableChBox->hide();
		asFretPosWdg->asNoteChB->hide();
		asFretPosWdg->asNameChB->hide();
		asFretPosWdg->asFretPosChB->hide();
		asFretPosWdg->asSoundChB->hide();
		asFretPosWdg->questLab->hide();
		asNoteWdg->asFretPosChB->hide();
		asNameWdg->asFretPosChB->hide();
		asSoundWdg->asFretPosChB->hide();
		showStrNrChB->hide();
		lowPosOnlyChBox->hide();
		m_asFretLab->hide();
		m_qGuitarNooLab->hide();
		m_guitarNooLab->hide();
}


void questionsSettings::saveLevel(Tlevel* level) {
    level->questionAs.setAsNote(asNoteWdg->isChecked());
    level->answersAs[TQAtype::e_asNote] = asNoteWdg->getAnswers();
    level->questionAs.setAsName(asNameWdg->isChecked());
    level->answersAs[TQAtype::e_asName] = asNameWdg->getAnswers();
    level->questionAs.setAsFret(asFretPosWdg->isChecked());
    level->answersAs[TQAtype::e_asFretPos] = asFretPosWdg->getAnswers();
    level->questionAs.setAsSound(asSoundWdg->isChecked());
    level->answersAs[TQAtype::e_asSound] = asSoundWdg->getAnswers();
    
    level->requireOctave = octaveRequiredChB->isChecked();
    level->requireStyle = styleRequiredChB->isChecked();
    level->showStrNr = showStrNrChB->isChecked();
    level->onlyLowPos = lowPosOnlyChBox->isChecked();
		level->intonation = m_intonationCombo->currentIndex();
		
		level->melodyLen = m_melodyLengthSpin->value();
		level->endsOnTonic = m_finishOnTonicChB->isChecked();
}


void questionsSettings::paintEvent(QPaintEvent* ) {
  QPainter painter(this);
  QPen pen = painter.pen();
	if (m_singleGr->isChecked())
		pen.setColor(palette().color(QPalette::Active, QPalette::Text));
	else
		pen.setColor(palette().color(QPalette::Disabled, QPalette::Text));
  pen.setWidth(1);
	pen.setStyle(Qt::DashLine);
  painter.setPen(pen);
	QPoint tl = m_singleGr->mapToParent(m_singleGr->contentsRect().topLeft());
  int vertLineUpY = tl.y() + m_questLab->geometry().y() + m_questLab->geometry().height() + 14;
  painter.drawLine(tl.x() + 10, vertLineUpY, tl.x() + m_singleGr->contentsRect().width() - 20, vertLineUpY);
  int vertLineDownY = tl.y() + asSoundWdg->enableChBox->geometry().y() + asSoundWdg->enableChBox->geometry().height() + 14;
  painter.drawLine(tl.x() + 10, vertLineDownY, tl.x() + m_singleGr->contentsRect().width() - 20, vertLineDownY);
  int horLineLeftX = tl.x() + asNoteWdg->enableChBox->x()	+ asNoteWdg->enableChBox->width() + 14;
  painter.drawLine(horLineLeftX, tl.y() + 10, horLineLeftX, tl.y() + m_singleGr->contentsRect().height() - 20);
  int horLineRightX = tl.x() + m_asSoundLab->geometry().x() + m_soundNooLab->geometry().width() + 14;
  painter.drawLine(horLineRightX, tl.y() + 10, horLineRightX, tl.y() + m_singleGr->contentsRect().height() - 20);
	
}


void questionsSettings::stringsCheckedSlot(bool checked) {
  if (checked)
    lowPosOnlyChBox->setDisabled(false);
  else {
    lowPosOnlyChBox->setChecked(false);
    lowPosOnlyChBox->setDisabled(true);    
  }
}


void questionsSettings::singleMultiSlot() {
	m_singleGr->blockSignals(true);
	m_melodiesGr->blockSignals(true);
	bool noMelody = false;
	if ((sender() == m_singleGr && m_singleGr->isChecked()) || (sender() == m_melodiesGr && !m_melodiesGr->isChecked()))
		      noMelody = true;
	if (noMelody) {
		m_melodiesGr->setChecked(false);
		m_singleGr->setChecked(true);
		m_playMelodyChB->setChecked(false);
		m_writeMelodyChB->setChecked(false);
		m_melodyLengthSpin->setRange(1, 1);
	} else {
		m_singleGr->setChecked(false);
		m_melodiesGr->setChecked(true);
		asNameWdg->setChecked(false);
		asFretPosWdg->setChecked(false);
		m_melodyLengthSpin->setRange(2, 50);
	}
	asSoundWdg->setChecked(false); // reset it either for melodies or for single note
	asNoteWdg->setChecked(false); // reset it either for melodies or for single note
	m_singleGr->blockSignals(false);
	m_melodiesGr->blockSignals(false);
}


void questionsSettings::melodyQuestionSlot() {
	if (m_playMelodyChB->isChecked()) {
			asNoteWdg->setAnswers(TQAtype(false, false, false, true));
			asNoteWdg->setChecked(true);
	} else {
			asNoteWdg->setChecked(false);
	}
	if (m_writeMelodyChB->isChecked()) {
			asSoundWdg->setAnswers(TQAtype(true, false, false, false));
			asSoundWdg->setChecked(true);
	} else {
			asSoundWdg->setChecked(false);
	}
}









