/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.7

import Score 1.0


Flickable {
  id: score

  property alias scoreObj: scoreObj
  property alias scale: staff0.scale
  property alias firstStaff: staff0
  property int clef: Tclef.Treble_G_8down
  property bool enableNoClef: true
  property alias meter: scoreObj.meter
  property alias bgColor: bgRect.color
  property bool enableKeySign: false
  property bool enableDoubleAccids: false
  property real scaleFactor: 1.0
  property alias notesCount: scoreObj.notesCount
  property TnoteItem currentNote: null

  // private
  property var staves: []

  clip: true
  width: parent.width

  contentWidth: score.width

  TscoreObject {
    id: scoreObj
    width: score.width / scale
    cursorAlter: accidControl.alter
    enableDoubleAccidentals: score.enableDoubleAccids

    onClicked: currentNote = scoreObj.activeNote

    onStaffCreate: {
      var c = Qt.createComponent("qrc:/Staff.qml")
      var lastStaff = c.createObject(score.contentItem, { "clef.type": score.clef })
      staves.push(lastStaff)
      lastStaff.enableKeySignature(enableKeySign && score.clef !== Tclef.NoClef)
      lastStaff.onDestroing.connect(removeStaff)
      if (enableKeySign && score.clef !== Tclef.NoClef) {
        lastStaff.keySignature.onKeySignatureChanged.connect(setKeySignature)
        lastStaff.keySignature.key = staff0.keySignature.key
      }
    }
    onStavesHeightChanged: score.contentHeight = Math.max(stavesHeight, score.height)
    onMeterChanged: enableNoClef = meter !== Tmeter.NoMeter

    function removeStaff(nr) { staves.splice(nr, 1) }
  }

  onCurrentNoteChanged: {
    if (currentNote && staves.length > 1)
      ensureVisible(currentNote.staffItem.y, currentNote.staffItem.height * scale)
  }

  Rectangle { // entire score background
    id: bgRect
    anchors.fill: score.contentItem
    color: Qt.rgba(activPal.base.r, activPal.base.g, activPal.base.b, 0.9)
  }

  Staff {
    id: staff0
    clef.type: score.clef
    meter: Meter { parent: staff0 }
    Component.onCompleted: staves.push(staff0)
  }

  NoteCursor {
    id: cursor
    parent: scoreObj.activeNote
    anchors.fill: parent
    yPos: scoreObj.activeYpos
    upperLine: scoreObj.upperLine
    alterText: accidControl.text
    z: 20
  }

  AccidControl {
    id: accidControl
    active: score.clef !== Tclef.NoClef && scoreObj.activeNote !== null
  }

  onEnableKeySignChanged: {
    staff0.enableKeySignature(enableKeySign)
    if (enableKeySign)
      staff0.keySignature.onKeySignatureChanged.connect(setKeySignature)
    for (var s = 1; s < staves.length; ++s) {
      staves[s].enableKeySignature(enableKeySign)
      if (enableKeySign)
        staff0.keySignature.onKeySignatureChanged.connect(setKeySignature)
    }
    scoreObj.keySignatureEnabled = enableKeySign
  }

  function staffChangesClef(staffId) {
    if (staffId.clef.type !== score.clef) {
      score.clef = staffId.clef.type
      for (var s = 0; s < staves.length; ++s) {
        if (staffId !== staves[s])
          staves[s].clef.type = staffId.clef.type
        staves[s].enableKeySignature(score.clef !== Tclef.NoClef && enableKeySign)
      }
      scoreObj.clefType = staffId.clef.type
    }
  }

  function setKeySignature(key) {
    if (enableKeySign) {
      for (var s = 0; s < staves.length; ++s) {
        if (key !== staves[s].keySignature.key)
          staves[s].keySignature.key = key
      }
      scoreObj.keySignature = key
    }
  }

  function ensureVisible(yy, hh) {
    if (contentY >= yy)
      contentY = yy;
    else if (contentY + height <= yy + hh)
      contentY = yy + hh - height;
  }

  function addNote(n) {
    scoreObj.addNote(n)
    var lastNote = scoreObj.note(scoreObj.notesCount - 1)
    if (staves.length > 1)
      ensureVisible(lastNote.staffItem.y, lastNote.staffItem.height * scale)
  }

  function setNote(staff, nr, n) { scoreObj.setNote(staff, nr, n) }
}
