/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017-2018 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2


import "../"

ListView {
  id: chList
  width: parent.width; height: parent.height


  model: dialogObj.getChanges()
  delegate: Tile {
    width: chList.width; height: relText.height + Noo.factor()
    bgColor: index % 2 ? activPal.base : activPal.alternateBase
    Text {
      id: relText
      text: modelData
      x: Noo.factor() / 2; y: Noo.factor() / 2
      width: chList.width - Noo.factor()
      horizontalAlignment: text.substring(0, 4) === "<h1>" ? Text.AlignHCenter : Text.AlignLeft
      color: activPal.text
      wrapMode: TextEdit.Wrap; textFormat: Text.StyledText
    }
  }

  ScrollBar.vertical: ScrollBar { active: true; visible: active }
}


