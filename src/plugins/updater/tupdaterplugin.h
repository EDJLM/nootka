/***************************************************************************
 *   Copyright (C) 2014 by Tomasz Bojczuk                                  *
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

#ifndef TUPDATERPLUGIN_H
#define TUPDATERPLUGIN_H

#include <QObject>
#include "../tplugininterface.h"

class TupdateChecker;

/** 
 * This plugin perform update checking for new Nootka versions.
 * It sends signals through @p ob QObject about current state of checking process
 */
class TupdaterPlugin : public QObject, public TpluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID TpluginInterface_iid FILE "")
  Q_INTERFACES(TpluginInterface)
  
  virtual ~TupdaterPlugin();
  
  virtual void init(const QString& argument = "", TpluginObject* ob = 0, QWidget* parent = 0, Texam* exam = 0);
  
  virtual QString& lastWord() { return m_lastWord; }
  
protected:
  void messageSlot(const QString& m) { m_sender->emitMessage(m); }
  
private:
  TpluginObject         *m_sender;
  TupdateChecker        *m_updater;
  QString                m_lastWord;
  
};


#endif // TUPDATERPLUGIN_H
