/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2018 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2

import "../"


Tile {
  property alias maxStep: maxSpin.value

  signal maxModified()

  description: qsTr("Maximal interval in semitones used in a melody.")
  Row {
    anchors.horizontalCenter: parent.horizontalCenter
    spacing: Noo.fontSize() / 2
    Text {
      anchors.verticalCenter: parent.verticalCenter
      text: qsTr("Max interval")
      color: activPal.text
    }
    SpinBox {
      id: maxSpin
      from: 2; to: 36
      value: 0
      onValueModified: maxModified()
    }
  }
}

