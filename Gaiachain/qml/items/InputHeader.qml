import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Items.GenericHeader {
    id: top

    property alias color: input.color
    property alias iconSource: input.iconSource
    property alias placeholderText: input.placeholderText
    property alias validator: input.validator
    property alias inputMethodHints: input.inputMethodHints

    property alias readOnly: input.readOnly

    property alias inputText: input.text
    property string suffixText

    property alias showIcon: input.showIcon
    signal iconClicked

    widget: Items.GenericInput {
        id: input

        Layout.fillWidth: true

        focus: false
        rightPadding: suffix.visible ? suffix.contentWidth + s(Style.hugeMargin) * 2
                                     : ((iconEdge === Enums.Edge.RightEdge) && iconItem.visible ? iconItem.width + 2 * iconItem.horizontalMargins
                                                                                                : padding)

        iconEdge: Enums.Edge.RightEdge

        onIconClicked: top.iconClicked()

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
