/***************************************************************************
 *   Copyright (C) 2017 by Tomasz Bojczuk                                  *
 *   seelook@gmail.com                                                     *
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

import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

import Nootka 1.0


ApplicationWindow {
  id: nootkaWindow
  visible: true
  title: "Nootka"
  color: activPal.window

  property alias fontSize: nootkaWindow.font.pixelSize
  property alias settingsAct: settingsAct
  property alias aboutAct: aboutAct
  property alias levelAct: levelAct
  property alias scoreAct: scoreAct
  property alias examAct: examAct

  SystemPalette { id: activPal; colorGroup: SystemPalette.Active }
  SystemPalette { id: disdPal; colorGroup: SystemPalette.Disabled }

  Taction {
    id: settingsAct
    icon: "systemsettings"
    text: qsTranslate("TtoolBar", "Settings")
    tip: qsTranslate("TtoolBar", "Application preferences")
    onTriggered: dialogLoader.page = Nootka.Settings
  }
  Taction { id: aboutAct; onTriggered: dialogLoader.page = Nootka.About }
  Taction {
    id: levelAct
    icon: "levelCreator"
    text: qsTranslate("TtoolBar", "Level")
    tip: qsTranslate("TtoolBar", "Levels creator")
  }
  Taction {
    id: scoreAct
    icon: "score"
    text: qsTranslate("TtoolBar", "Score", "it could be 'notation', 'staff' or whatever is associated with that 'place to display musical notes' and this the name is quite short and looks well.")
    tip: qsTranslate("TtoolBar", "Manage and navigate the score.")
    onTriggered: score.scoreMenu.open()
  }
  Taction {
    id: examAct
    icon: "startExam"
    text: qsTranslate("TtoolBar", "Lessons")
    tip: qsTranslate("TtoolBar", "Start exercises or an exam")
    onTriggered: randNotes()
  }

  width: GLOB.geometry.width
  height: GLOB.geometry.height
  x: GLOB.geometry.x
  y: GLOB.geometry.y

  onClosing: GLOB.geometry = Qt.rect(x ,y, width, height)

  MainMenu { id: mainMenu }
  header: mainMenu.toolBar

  Column {
      anchors.fill: parent

      Row {
        visible: !Noo.isAndroid()
        height: nootkaWindow.height / 12
        width: nootkaWindow.width
        Label {
          id: statLab
          text: "Bla bla bla"
          verticalAlignment: Qt.AlignVCenter
          horizontalAlignment: Qt.AlignHCenter
          height: parent.height
          width: parent.width * 0.6
          color: activPal.text
        }

        PitchView {
          id: pitchView
          height: parent.height
          width: parent.width * 0.4
        }
      }

      MainScore {
        id: score
        height: nootkaWindow.height * (Noo.isAndroid() ? 1.0 : 0.916667) - (header ? header.height : 0) - instrument.height
        scoreMenu.x: header ? header.scoreAct.x : 0
      }

      Instrument {
        id: instrument
        height: nootkaWindow.height / 4
        width: parent.width

      }
  }

  DialogLoader { id: dialogLoader }

//   Component.onCompleted: {
//   }

//   Timer {
//       interval: 5000
//       running: true
//       repeat: true
//       onTriggered: {
//         score.addNote(Noo.note(1 + Math.random() * 7, -2 + Math.random() * 5, Math.min(Math.max(-2, -3 + Math.random() * 6), 2),
//                                2 + Math.random() * 4))
//         var noteNr = Math.random() * 7
//         var rest = Math.floor((Math.random() * 100) % 2)
//         var accid = rest ? 0 : Math.min(Math.max(-2, -3 + Math.random() * 6), 2)
//         var note = Noo.note(1 + Math.random() * 7, -3 + Math.random() * 7, accid, 1 + Math.random() * 5, rest)
//         score.setNote(0, noteNr, note)
//         score.enableKeySign = !score.enableKeySign
//         if (score.enableKeySign)
//           score.setKeySignature(-7 + Math.random() * 15)
//         var m = Math.pow(2, Math.floor(1 + Math.random() * 11))
//         console.log("meter " + m)
//         score.meter = m
//       }
//   }
  function randNotes() {
    var rest = (Math.random() * 100)  > 90
//     var accid = rest ? 0 : Math.min(Math.max(-2, -3 + Math.random() * 6), 2)
    var accid = rest ? 0 : Math.min(Math.floor(Math.random() * 2), 1)
    var note = rest ? 0 : 1 + Math.random() * 7
    var octave = -2 + Math.random() * 5
    var rtm =  /*Math.random() * 100 > 50 ? 4 : 5 */ 2 + Math.random() * 4
    score.addNote(Noo.note(note, octave, accid, rtm, rest))
//     var noteNr = Math.random() * 7
//     var rest = Math.floor((Math.random() * 100) % 2)
//     var note = Noo.note(1 + Math.random() * 7, -3 + Math.random() * 7, accid, 1 + Math.random() * 5, rest)
//     score.setNote(0, noteNr, note)
  }

}
