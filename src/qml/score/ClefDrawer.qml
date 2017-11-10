/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2

// import Score 1.0


Drawer {
  property alias selectedClef: clefMenu.selClef
  width: Noo.fontSize() * 18
  height: nootkaWindow.height
  visible: true
  Flickable {
    id: flick
    anchors.fill: parent
    clip: true
    contentHeight: clefMenu.height
    ClefMenu {
      id: clefMenu
      onClicked: { type = cl; close() }
      onSelIdChanged: { // ensure visible
        var yy = Noo.fontSize() * 7.75 * selId
        if (flick.contentY >= yy)
          flick.contentY = yy
          else if (flick.contentY + flick.height <= yy + Noo.fontSize() * 7.75)
            flick.contentY = yy + Noo.fontSize() * 7.75 - flick.height
      }
    }
  }
}
