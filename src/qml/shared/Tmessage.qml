/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2021 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Dialogs 1.2


TpopupDialog {
  property alias message: mText.text
  property color accent: activPal.highlight

  bgColor: Qt.tint(activPal.base, NOO.alpha(accent, 20))
  border { color: accent; width: NOO.factor() / 4.0 }
  glowRect.radius: NOO.factor()
  visible: true; modal: true

  width: mText.width + NOO.factor() / 2
  height: mText.height + header.height + footer.height + NOO.factor()

  rejectButton.visible: false
  acceptButton.text: NOO.TR("QPlatformTheme", "OK")

  Text {
    id: mText
    x: NOO.factor() //; y: (parent.height - height) / 2
    width: NOO.factor() * 40
    wrapMode: Text.WordWrap
    horizontalAlignment: Text.AlignHCenter
  }

  onClosed: destroy()
}
