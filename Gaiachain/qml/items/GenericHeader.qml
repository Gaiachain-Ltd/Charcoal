import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    readonly property bool headerIsEmpty: (headerText === Strings.empty)

    property alias widget: mainLayout.data
    property string headerText

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: headerIsEmpty ? Style.none : s(Style.smallMargin)

        Items.BasicText {
            Layout.fillWidth: true

            font.bold: true
            horizontalAlignment: Text.AlignLeft

            visible: !headerIsEmpty

            text: headerText
        }
    }
}
