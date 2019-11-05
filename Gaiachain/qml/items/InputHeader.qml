import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    property alias color: input.color
    property alias iconSource: input.iconSource

    property string headerText
    property string inputText
    property string suffixText

    property bool showIcon: false

    readonly property bool headerIsEmpty: (headerText === Strings.empty)

    signal iconClicked()

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

        Items.GenericInput {
            id: input

            Layout.fillWidth: true

            readOnly: true

            rightPadding: suffix.visible ? suffix.contentWidth + s(Style.hugeMargin) * 2 : padding

            showIcon: top.showIcon
            iconEdge: Enums.Edge.RightEdge
            onIconClicked: top.iconClicked()

            text: inputText

            Items.BasicText {
                id: suffix

                anchors {
                    right: parent.right
                    rightMargin: s(Style.hugeMargin)
                    top: parent.top
                    bottom: parent.bottom
                }

                font: parent.font
                color: parent.color
                visible: (suffixText !== Strings.empty && !showIcon)

                text: suffixText
            }
        }
    }
}
