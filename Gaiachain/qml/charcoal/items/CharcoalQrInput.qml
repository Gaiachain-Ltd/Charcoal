import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

import "../../items" as Items

import com.gaiachain.style 1.0
import com.gaiachain.static 1.0

Item {
  id: root

  readonly property string separator: "-"
  readonly property string qrCode: qrPart1.text + separator + qrPart2.text + separator + qrPart3.text

  height: qrPart1.height

  Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

  onVisibleChanged: {
    if (visible) {
      qrPart1.forceActiveFocus()
    }
  }

  function clear() {
    qrPart1.clear()
    qrPart2.clear()
    qrPart3.clear()
    qrPart3.focus = false
    qrPart2.focus = false
    qrPart1.focus = true
  }

  Rectangle {
    id: borderRectangle

    anchors.centerIn: parent
    height: qrPart1.height
    width: row.width
    radius: s(GStyle.tinyMargin)
    color: GStyle.blank
    border.width: sr(GStyle.controlDefaultBorderWidth)
    border.color: GStyle.inputBorderColor

    Row {
      id: row

      anchors.centerIn: parent
      height: parent.height
      padding: 0
      spacing: 0

      CharcoalQrSectionInput {
        id: qrPart1
        nextInput: qrPart2
      }

      Text {
        id: spacerText
        anchors.verticalCenter: parent.verticalCenter
        text: root.separator
        font.pixelSize: qrPart1.fontSize
        font.family: GStyle.primaryFontFamily
        height: root.height
        verticalAlignment: Qt.AlignVCenter
      }

      CharcoalQrSectionInput {
        id: qrPart2
        previousInput: qrPart1
        nextInput: qrPart3
      }

      Text {
        anchors.verticalCenter: parent.verticalCenter
        text: root.separator
        font.pixelSize: qrPart1.fontSize
        font.family: GStyle.primaryFontFamily
        height: root.height
        verticalAlignment: Qt.AlignVCenter
      }

      CharcoalQrSectionInput {
        id: qrPart3
        previousInput: qrPart2
      }
    }
  }
}
