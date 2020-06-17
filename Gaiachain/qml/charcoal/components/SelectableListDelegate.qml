import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../../items" as Items

// TODO: reuse this component in OvenTypeComboBox and regular ComboBox
Item {
    property alias text: delegateText.text
    property alias textColor: delegateText.color
    property url checkIcon: GStyle.checkBlackUrl
    property bool selected: false
    property int textLeftMargin: 0

    signal clicked()

    onClicked: {
        selected = !selected
    }

    id: delegateItem
    height: layout.implicitHeight

    ColumnLayout {
        id: layout
        anchors.fill: parent

        RowLayout {
            Layout.leftMargin: delegateItem.textLeftMargin

            Items.GText {
                Layout.fillWidth: true

                id: delegateText
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.bold: true
                padding: s(GStyle.middleMargin)
            }

            Image {
                Layout.rightMargin: s(GStyle.middleMargin)
                source: checkIcon
                visible: selected
            }
        }

        Rectangle {
            Layout.fillWidth: true

            color: GStyle.separatorColor
            height: 1
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: delegateItem.clicked()
    }
}
