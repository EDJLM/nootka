/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017-2021 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9

import Nootka 1.0
import Nootka.Exam 1.0

import "../"


Texecutor {
  id: executor

  parent: NOO.isAndroid() ? nootkaWindow.contentItem : nootkaWindow.contentItem.parent
  z: 100

  //private
  property var examSettDialog: null
  property var helpGotIt: null

  anchors.fill: parent

  onTitleChanged: nootkaWindow.title = title

  onExamActionsChanged: {
    if (!NOO.isAndroid())
      nootkaWindow.mainMenu.toolBar.examActions = examActions
  }

  onExamSummary: nootkaWindow.showDialog(Nootka.ExamSummary)

  onShowSettings: {
    if (!examSettDialog) {
      var e = Qt.createComponent("qrc:/exam/ExamSettingsDialog.qml")
      examSettDialog = e.createObject(executor, { "mode": isExercise ? 2 : 1 } )
      examSettDialog.accepted.connect(settingsAccepted)
      examSettDialog.closed.connect(function() { examSettDialog.destroy() })
    }
  }

  onWantMessage: {
    var ms = Qt.createComponent("qrc:/Tmessage.qml").createObject(nootkaWindow.contentItem.parent, { "caption": caption, "message": message, "accent": accent })
    ms.closed.connect(afterMessage)
  }

  onExecDiscarded: {
    console.log("[ExamExecutor] Executor discarded, deleting it")
    GLOB.isExam = false
    nootkaWindow.executor.destroy()
  }

  onShowHelp: {
    if (!helpGotIt)
      helpGotIt = Qt.createComponent("qrc:/gotit/ExamFlow.qml").createObject(executor, { "remaindChecked": showExamHelp })
    helpGotIt.open()
    helpGotIt.closed.connect(afterMessage)
  }

  onWantSuggestPopup: Qt.createComponent("qrc:/exam/TsuggestExam.qml").createObject(executor, { "entireVisible": showEntire })

  Component.onDestruction: {
    if (helpGotIt)
      GLOB.setGotIt("examFlow", helpGotIt.remaindChecked)
  }

  Connections {
    target: tipHandler
    onWantStartTip: {
      var s = Qt.createComponent("qrc:/exam/ExamTip.qml")
      tipHandler.startTip = s.createObject(executor, { "text": text, "offX": pos.x, "offY": pos.y, "bg": color } )
    }
    onWantQuestionTip: {
      var s = Qt.createComponent("qrc:/exam/QuestionTip.qml")
      tipHandler.questionTip = s.createObject(executor, { "text": text, "offX": pos.x, "offY": pos.y } )
    }
    onWantConfirmTip: {
      var s = Qt.createComponent("qrc:/exam/ExamTip.qml")
      tipHandler.confirmTip = s.createObject(executor, { "text": text, "offX": pos.x, "offY": pos.y, "bg": color, "showExit": !NOO.isAndroid() } )
    }
    onWantResultTip: {
      var r = Qt.createComponent("qrc:/exam/ResultTip.qml")
      tipHandler.resultTip = r.createObject(nootkaWindow, { "text": text, "color": color } )
    }
    onWantTryAgainTip: {
      var a = Qt.createComponent("qrc:/exam/ResultTip.qml")
      tipHandler.tryAgainTip = a.createObject(nootkaWindow, { "text": qsTranslate("TtipHandler", "Try again!"),
                                                            "color": GLOB.wrongColor, "targetY": executor.height / 12 } )
    }
    onWantWhatNextTip:  {
      var s = Qt.createComponent("qrc:/exam/ExamTip.qml")
      tipHandler.whatNextTip = s.createObject(executor, { "text": text, "offX": pos.x, "offY": pos.y, "bg": color } )
    }
    onWantCertificate: {
      tipHandler.certTip = Qt.createComponent("qrc:/exam/Certificate.qml").createObject(nootkaWindow.contentItem.parent)
    }
  }
}

