import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Items.GenericHeader {
    id: top

    property alias color: input.color
    property url iconSource
    property alias placeholderText: input.placeholderText
    property alias validator: input.validator
    property alias additionalInputMethodHints: input.additionalInputMethodHints

    property alias readOnly: input.readOnly

    property alias inputText: input.text
    property string suffixText

    property bool optional: false
    readonly property bool isEmpty: (inputText === Strings.empty)

    property alias showIcon: input.showIcon
    signal iconClicked

    function clear() {
        inputText = Strings.empty
    }

    widget: Items.GenericInput {
        id: input

        Layout.fillWidth: true

        readonly property real iconItemSpacing: iconItem.visible ? iconItem.width + 2 * iconItem.horizontalMargins : Style.none
        readonly property real suffixItemSpacing: suffix.visible ? suffix.contentWidth + s(Style.hugeMargin) * 2 : Style.none
        readonly property real itemsSpacing: (iconItemSpacing + suffixItemSpacing)

        focus: false
        rightPadding: (itemsSpacing === Style.none ? padding : itemsSpacing)

        iconEdge: Enums.Edge.RightEdge

        iconSource: optional && !isEmpty ? Style.clearImgUrl : top.iconSource
        onIconClicked: {
            if (optional && !isEmpty) {
                top.clear()
            } else {
                top.iconClicked()
            }
        }

        Items.BasicText {
            id: suffix

            anchors {
                right: parent.right
                rightMargin: s(Style.hugeMargin) + input.iconItemSpacing
                top: parent.top
                bottom: parent.bottom
            }

            font: parent.font
            color: parent.color
            visible: (suffixText !== Strings.empty)

            text: suffixText
        }
    }
}
