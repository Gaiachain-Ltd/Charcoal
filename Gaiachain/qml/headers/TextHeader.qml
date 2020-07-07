import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.GHeader {
    id: top

    property url iconSource
    //property alias placeholderText: input.placeholderText
    //property alias validator: input.validator
    //property alias additionalInputMethodHints: input.additionalInputMethodHints

    //property alias readOnly: input.readOnly

    property alias text: textWidget.text
    property string suffixText

    property bool optional: false
    readonly property bool isEmpty: (inputText === Strings.empty)
    //property alias borderWidth: textWidget.borderWidth

    //property alias showIcon: textWidget.showIcon
    //signal iconClicked

    property bool forceBoldValue: false
    property int valueFontSize: s(GStyle.inputPixelSize)

    function clear() {
        inputText = Strings.empty
    }

    widget: Items.GText {
        id: textWidget

        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        readonly property real iconItemSpacing: iconItem.visible ? iconItem.width + 2 * iconItem.horizontalMargins : GStyle.none
        readonly property real suffixItemSpacing: suffix.visible ? suffix.contentWidth + s(GStyle.hugeMargin) * 2 : GStyle.none
        readonly property real itemsSpacing: (iconItemSpacing + suffixItemSpacing)

        focus: false
        rightPadding: (itemsSpacing === GStyle.none ? padding : itemsSpacing)

        //backgroundColor: top.backgroundColor
        color: secondaryTextColor
        font.bold: top.highlighted || forceBoldValue
        font.pixelSize: valueFontSize
        //borderWidth: top.summaryMode? 0 : sr(1)

        wrapMode: Text.WrapAnywhere

        //iconEdge: Enums.Edge.RightEdge

        //iconSource: optional && !isEmpty ? GStyle.clearImgUrl : top.iconSource
//        onIconClicked: {
//            if (optional && !isEmpty) {
//                top.clear()
//            } else {
//                top.iconClicked()
//            }
//        }

//        Items.GText {
//            id: suffix

//            anchors {
//                right: parent.right
//                rightMargin: s(GStyle.hugeMargin) + input.iconItemSpacing
//                top: parent.top
//                bottom: parent.bottom
//            }

//            font: parent.font
//            color: parent.color
//            visible: (suffixText !== Strings.empty)
//            palette {
//                base: parent.backgroundColor
//            }

//            text: suffixText
//        }
    }
}
