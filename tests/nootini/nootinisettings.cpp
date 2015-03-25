/***************************************************************************
 *   Copyright (C) 2015 by Tomasz Bojczuk                                  *
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

#include "nootinisettings.h"
#include <tinitcorelib.h>
#include <taudioparams.h>
#include <tartiniparams.h>
#include <tpitchfinder.h>
#include <widgets/tintonationview.h>
#include <../settings/tvolumeslider.h>
#include <QtWidgets/QtWidgets>


NootiniSettings::NootiniSettings(TartiniParams* tp, QWidget* parent) :
  QDialog(parent),
  m_tartiniParams(tp)
{
  setWindowTitle(tr("Parameters of processing"));
  setWindowIcon(parent->windowIcon());

  QGroupBox *modeGr = new QGroupBox(tr("pitch detection mode"), this);
  m_mpmRadio = new QRadioButton("MPM", this);
  m_correlRadio = new QRadioButton("autocorrelation", this);
  m_cepstrumRadio = new QRadioButton("MPM + modified cepstrum", this);
  QButtonGroup *butGr = new QButtonGroup(this);
  butGr->addButton(m_mpmRadio);
  butGr->addButton(m_correlRadio);
  butGr->addButton(m_cepstrumRadio);
  if (Tcore::gl()->A->detectMethod == e_MPM)
    m_mpmRadio->setChecked(true);
  else if (Tcore::gl()->A->detectMethod == e_AUTOCORRELATION)
    m_correlRadio->setChecked(true);
  else
    m_cepstrumRadio->setChecked(true);

  QLabel *durHeadLab = new QLabel(tr("minimum note duration"), this);
  m_durationSpin = new QSpinBox(this);
    m_durationSpin->setMinimum(10);
    m_durationSpin->setMaximum(1000);
    m_durationSpin->setSuffix("   "  + tr("[milliseconds]"));
    m_durationSpin->setSingleStep(50);
    m_durationSpin->setValue(qRound(Tcore::gl()->A->minDuration * 1000)); // minimum duration is stored in seconds but displayed in milliseconds

  QLabel *volLabel = new QLabel(tr("minimum volume"), this);
  m_volumeSlider = new TvolumeSlider(this);
    m_volumeSlider->setValue(Tcore::gl()->A->minimalVol);

  m_lowRadio = new QRadioButton(tr("low")  + " (2048)", this);
  m_middleRadio = new QRadioButton(tr("middle") + " (1024)", this);
  m_highRadio = new QRadioButton(tr("high") + " (512)", this);
  QButtonGroup *rangeGr = new QButtonGroup(this);
    rangeGr->addButton(m_lowRadio);
    rangeGr->addButton(m_middleRadio);
    rangeGr->addButton(m_highRadio);

  QLabel *frLab = new QLabel(tr("frequency:"), this);
  m_freqSpin = new QSpinBox(this);
    m_freqSpin->setMinimum(200);
    m_freqSpin->setMaximum(900);
    m_freqSpin->setSuffix(" Hz");
    m_freqSpin->setValue(qRound((pitch2freq(freq2pitch(440) + Tcore::gl()->A->a440diff))));

  QLabel *threshLab = new QLabel(tr("threshold of lowest loudness (MPM methods)"), this);
  m_thresholdSpin = new QSpinBox(this);
    m_thresholdSpin->setRange(80, 100);
    m_thresholdSpin->setSuffix(" %");
    m_thresholdSpin->setValue(m_tartiniParams->threshold);

  m_noiseFilterChB = new QCheckBox(tr("noise filter"), this);
    m_noiseFilterChB->setChecked(Tcore::gl()->A->equalLoudness);

  m_calcNoiseChB = new QCheckBox(tr("Automatically calculate noise-floor"), this);
    m_calcNoiseChB->setChecked(m_tartiniParams->doingAutoNoiseFloor);

//   m_nootkaIndexChB = new QCheckBox(tr("Nootka processing"), this);

  m_splitVolGroup = new QGroupBox(tr("split on volume ascent"), this);
    m_splitVolGroup->setCheckable(true);
  m_splitVolSpin = new QDoubleSpinBox(this);
    m_splitVolSpin->setRange(0.05, 0.5);
    m_splitVolSpin->setSingleStep(0.05);
    m_splitVolSpin->setSuffix(" %");
  setMinVolToSplit(Tcore::gl()->A->minSplitVol);

  QLabel *dbLab = new QLabel(tr("dbFloor"), this);
  m_dbFlorSpin = new QDoubleSpinBox(this);
    m_dbFlorSpin->setRange(-300, 0);
    m_dbFlorSpin->setValue(m_tartiniParams->dBFloor);

  m_drawVolChB = new QCheckBox(tr("draw volume chart"), this);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

  QVBoxLayout *lay = new QVBoxLayout;
  QHBoxLayout *methodLay = new QHBoxLayout;
    methodLay->addWidget(m_mpmRadio);
    methodLay->addWidget(m_correlRadio);
    methodLay->addWidget(m_cepstrumRadio);
  modeGr->setLayout(methodLay);
  lay->addWidget(modeGr);
  QHBoxLayout *durLay = new QHBoxLayout;
    durLay->addStretch();
    durLay->addWidget(durHeadLab);
    durLay->addWidget(m_durationSpin, 0, Qt::AlignLeft);
    durLay->addStretch();
  lay->addLayout(durLay);
  QHBoxLayout *volLay = new QHBoxLayout;
    volLay->addWidget(volLabel);
    volLay->addWidget(m_volumeSlider);
  lay->addLayout(volLay);
  QHBoxLayout *rangeLay = new QHBoxLayout;
    rangeLay->addWidget(m_lowRadio);
    rangeLay->addWidget(m_middleRadio);
    rangeLay->addWidget(m_highRadio);
  QGroupBox *rangeBox = new QGroupBox(tr("Range of note pitches:"), this);
    rangeBox->setLayout(rangeLay);
  lay->addWidget(rangeBox);
  QHBoxLayout *freqLay = new QHBoxLayout;
    freqLay->addStretch();
    freqLay->addWidget(frLab);
    freqLay->addWidget(m_freqSpin);
    freqLay->addStretch();
  lay->addLayout(freqLay);
  QHBoxLayout *threshLay = new QHBoxLayout;
    threshLay->addStretch();
    threshLay->addWidget(threshLab);
    threshLay->addWidget(m_thresholdSpin);
    threshLay->addStretch();
  lay->addLayout(threshLay);
  QHBoxLayout *noiseLay = new QHBoxLayout;
    noiseLay->addStretch();
    noiseLay->addWidget(m_noiseFilterChB);
    noiseLay->addStretch();
    noiseLay->addWidget(m_calcNoiseChB);
    noiseLay->addStretch();
  lay->addLayout(noiseLay);
    QVBoxLayout *splitLay = new QVBoxLayout;
    splitLay->addWidget(m_splitVolSpin, 1, Qt::AlignCenter);
    m_splitVolGroup->setLayout(splitLay);
  lay->addWidget(m_splitVolGroup);
    QHBoxLayout *dbLay = new QHBoxLayout;
    dbLay->addStretch();
    dbLay->addWidget(dbLab);
    dbLay->addWidget(m_dbFlorSpin);
    dbLay->addStretch();
  lay->addLayout(dbLay);
  QHBoxLayout *nootLay = new QHBoxLayout;
//     nootLay->addStretch();
//     nootLay->addWidget(m_nootkaIndexChB);
    nootLay->addStretch();
    nootLay->addWidget(m_drawVolChB);
    nootLay->addStretch();
  lay->addLayout(nootLay);
  lay->addWidget(buttonBox);
  setLayout(lay);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}


bool NootiniSettings::nootkaIndexing() {
//   return m_nootkaIndexChB->isChecked();
  return true;
}


void NootiniSettings::setNootkaIndexing(bool yes) {
//   m_nootkaIndexChB->setChecked(yes);
}


void NootiniSettings::setMinVolToSplit(qreal minVol) {
  if (minVol >= 0.01) {
    m_splitVolGroup->setChecked(true);
    m_splitVolSpin->setValue(minVol);
  } else
    m_splitVolGroup->setChecked(false);
}


qreal NootiniSettings::minVolToSplit() {
  if (m_splitVolGroup->isChecked())
    return m_splitVolSpin->value();
  else
    return 0.0;
}


bool NootiniSettings::drawVolumeChart() {
  return m_drawVolChB->isChecked();
}


void NootiniSettings::setDrawVolumeChart(bool draw) {
  m_drawVolChB->setChecked(draw);
}


void NootiniSettings::setRange(int r) {
  TpitchFinder::Erange rang = (TpitchFinder::Erange)r;
  switch (rang) {
    case TpitchFinder::e_high:
      m_highRadio->setChecked(true); break; // e_high - lowest pitch is F small
    case TpitchFinder::e_low:
      m_lowRadio->setChecked(true); break; // e_low - lowest pitch is ... very low
    default:
      m_middleRadio->setChecked(true); break; // e_middle - lowest pitch is F contra
  }
}


int NootiniSettings::range() {
  if (m_highRadio->isChecked())
    return TpitchFinder::e_high;
  else if (m_lowRadio->isChecked())
    return TpitchFinder::e_low;
  else
    return TpitchFinder::e_middle;
}



//#################################################################################################
//###################              PROTECTED           ############################################
//#################################################################################################
void NootiniSettings::accept() {
  if (m_mpmRadio->isChecked())
    Tcore::gl()->A->detectMethod = 0;
  else if (m_correlRadio->isChecked())
    Tcore::gl()->A->detectMethod = 1;
  else
    Tcore::gl()->A->detectMethod = 2;
  Tcore::gl()->A->minDuration = (qreal)m_durationSpin->value() / 1000.0;
  Tcore::gl()->A->minimalVol = m_volumeSlider->value();
  if (m_freqSpin->value() == 440 )
      Tcore::gl()->A->a440diff = 0.0;
  else
      Tcore::gl()->A->a440diff = float(freq2pitch((double)m_freqSpin->value()) - freq2pitch(440.0));

  m_tartiniParams->threshold = m_thresholdSpin->value();
  Tcore::gl()->A->equalLoudness = m_noiseFilterChB->isChecked();
  Tcore::gl()->A->minSplitVol = minVolToSplit();
  m_tartiniParams->dBFloor = m_dbFlorSpin->value();
  m_tartiniParams->doingAutoNoiseFloor = m_calcNoiseChB->isChecked();

  QDialog::accept();
}






