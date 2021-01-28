/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2020-2021 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9

import "../"


TpopupDialog {
  property alias message: lv.model
  property color accent: activPal.highlight

  bgColor: Qt.tint(activPal.base, Noo.alpha(accent, 20))
  border { color: accent; width: Noo.factor() / 4.0 }
  visible: true; modal: true
  width: lv.width + Noo.factor() / 2
  height: lv.height + header.height + footer.height + Noo.factor()

  rejectButton.visible: false
  acceptButton.text: Noo.TR("QPlatformTheme", "OK")
  acceptButton.visible: !Noo.isAndroid()

  ListView {
    id: lv
    width: Noo.factor() * 40; height: contentHeight
    spacing: 2; clip: true
    delegate: Rectangle {
      width: lv.width; height: mText.height + Noo.factor() / 2
      color: index % 2 === 1 ? "transparent" : Noo.alpha(activPal.base, 50)
      Rectangle {
        width: Noo.factor() / 2; height: width; radius: width / 2
        x: Noo.factor() / 2; y: (parent.height - height) / 2
        color: activPal.text
      }
      Text {
        id: mText
        x: Noo.factor() * 1.5; y: (parent.height - height) / 2
        width: lv.width - Noo.factor() * 2
        wrapMode: Text.WordWrap
        text: modelData
        font.pixelSize: Noo.factor()
      }
    }
  }

  onClosed: destroy()
}
