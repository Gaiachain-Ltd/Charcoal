import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    readonly property bool headerIsEmpty: (headerText === Strings.empty)

    property alias widget: mainLayout.data

    property alias headerText: header.text
    property alias headerTextColor: header.color

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: headerIsEmpty ? GStyle.none : s(GStyle.smallMargin)

        Items.GText {
            id: header

            Layout.fillWidth: true

            font.bold: true
            horizontalAlignment: Text.AlignLeft

            visible: !headerIsEmpty
            wrapMode: Text.WordWrap
        }
    }
}
