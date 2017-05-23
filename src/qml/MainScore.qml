/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.7
import QtQuick.Controls 2.0

import Score 1.0
import Nootka 1.0
import "fakeTrans.js" as Fake


Score {
  id: mainScore

  width: parent.width

  property alias showNamesAct: showNamesAct
  property alias extraAccidsAct: extraAccidsAct
  property alias zoomInAct: zoomInAct
  property alias zoomOutAct: zoomOutAct
  property alias openXmlAct: openXmlAct
  property alias saveXmlAct: saveXmlAct

  meter: GLOB.rhythmsEnabled ? Tmeter.Meter_4_4 : Tmeter.NoMeter
  enableNoClef: GLOB.rhythmsEnabled

  clef: GLOB.clefType
  scoreObj.clefType: GLOB.clefType
  enableDoubleAccids: GLOB.enableDoubleAccids
  enableKeySign: GLOB.keySignatureEnabled
  scoreObj.showNoteNames: GLOB.namesOnScore
  scoreObj.nameColor: GLOB.nameColor
  scoreObj.nameStyle: GLOB.noteNameStyle
  scoreObj.enableDoubleAccidentals: GLOB.enableDoubleAccids
  scoreObj.allowAdding: true

  Text {
    id: keyName
    parent: firstStaff
    visible: GLOB.showKeyName && enableKeySign
    x: 4.5
    y: 5
    color: activPal.text
    font.pointSize: 1.5
    text: getKeyNameText()
    Connections {
      target: GLOB
      onKeyNameChanged: keyName.text = Qt.binding(keyName.getKeyNameText) //Noo.majAndMinKeyName(firstStaff.keySignature.key)
    }
    function getKeyNameText() {
      return enableKeySign ? Noo.majAndMinKeyName(firstStaff.keySignature.key) : ""
    }
  }

  Rectangle { // note highlight
    id: noteHighlight
    parent: currentNote
    visible: currentNote != null
    width: currentNote ? (currentNote.width - currentNote.alterWidth) * 1.5 : 0
    height: currentNote ? Math.min(12.0, currentNote.notePosY + 6.0) : 0
    x: currentNote ? -width * 0.25 : 0
    y: currentNote ? Math.min(currentNote.height - height, Math.max(0.0, currentNote.notePosY - height / 2.0)) : 0
    color: Qt.rgba(activPal.highlight.r, activPal.highlight.g, activPal.highlight.b, 0.3)
    z: -1
    radius: width / 3.0
  }

//   NoteName {
//     width: mainScore.width / 4
//     height: mainScore.scale * 32
//     y: mainScore.scale * 0.4
//     x: mainScore.width * 0.7
//   }

  Taction {
    id: openXmlAct
    text: Fake.tr("QShortcut", "Open")
    icon: "open"
    onTriggered: {
      var xmlFle = Noo.getXmlToOpen()
      scoreObj.openMusicXml(xmlFle)
    }
    shortcut: Shortcut { sequence: StandardKey.Open; onActivated: openXmlAct.triggered() }
  }
  Taction {
    id: saveXmlAct
    text: Fake.tr("QShortcut", "Save")
    icon: "save"
    onTriggered: {
      var xmlFle = Noo.getXmlToSave()
      scoreObj.saveMusicXml(xmlFle)
    }
    shortcut: Shortcut { sequence: StandardKey.Save; onActivated: saveXmlAct.triggered() }
  }
  Taction {
    id: extraAccidsAct
    text: qsTr("Additional accidentals")
    checkable: true
  }
  Taction {
    id: showNamesAct
    text: qsTr("Show note names")
    checkable: true
    checked: GLOB.namesOnScore
    onTriggered: { scoreObj.showNoteNames = checked }
  }
  Taction {
    id: zoomOutAct
    icon: "zoom-out"
    text: qsTr("Zoom score out")
    onTriggered: scaleFactor = Math.max(0.4, scaleFactor - 0.2)
    shortcut: Shortcut { sequence: StandardKey.ZoomOut; onActivated: zoomOutAct.triggered() }
  }
  Taction {
    id: zoomInAct
    icon: "zoom-in"
    text: qsTr("Zoom score in")
    onTriggered: scaleFactor = scaleFactor = Math.min(scaleFactor + 0.2, 1.4)
    shortcut: Shortcut { sequence: StandardKey.ZoomIn; onActivated: zoomInAct.triggered() }
  }

  Shortcut {
    sequence: StandardKey.MoveToNextChar;
    onActivated: {
      if (currentNote) {
          if (currentNote.index < notesCount - 1)
            currentNote =  scoreObj.note(currentNote.index + 1)
      } else
          currentNote = scoreObj.note(0)
    }
  }
  Shortcut {
    sequence: StandardKey.MoveToPreviousChar;
    onActivated: {
      if (currentNote) {
          if (currentNote.index > 0)
            currentNote = scoreObj.note(currentNote.index - 1)
      } else {
          currentNote = scoreObj.note(0)
      }
    }
  }

}
