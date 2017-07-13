/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2


Item {
  id: nootkaLabel
  property color bgColor: "white"
  property alias active: mouseArea.hoverEnabled

  signal clicked()

  width: logo.width

  Rectangle {
    id: bgRect
    height: parent.height
    width: logo.width
    color: bgColor
  }

  Image {
    id: logo
    smooth: true
    anchors.top: parent.Top
    source: Noo.pix("logo")
    sourceSize.height: nootkaLabel.height
  }

  Text {
    y: nootkaLabel.height * 0.72
    anchors.horizontalCenter: logo.horizontalCenter
    font.pixelSize: nootkaLabel.height * 0.2
    color: bgColor
    text: Noo.version()
  }

  MouseArea {
    id: mouseArea
    anchors.fill: parent
    hoverEnabled: !Noo.isAndroid()
    onEntered: bgColor = Noo.randomColor()
    onExited: bgColor = "white"
    onClicked: nootkaLabel.clicked()
  }
}
