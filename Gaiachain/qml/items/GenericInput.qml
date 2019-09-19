import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0

TextField
{
    id: top

    property bool isPassword: false
    property int additionalInputMethodHints: 0

    property alias iconSource: icon.source
    property bool showIcon: true

    property Item nextInput: null
    signal moveToNextInput()

    onMoveToNextInput: {
        if (nextInput !== null)
            nextInput.focus = true
    }

    focus: true
    autoScroll: true
    maximumLength: 50
    padding: s(Style.bigMargin)
    leftPadding: icon.visible ? icon.width + 2 * icon.horizontalMargins : padding

    font {
        pixelSize: s(Style.inputPixelSize)
        family: Style.primaryFontFamily
    }
    placeholderTextColor: Style.textPrimaryColor

    echoMode: isPassword ? TextInput.Password : TextInput.Normal
    inputMethodHints: Qt.ImhNoPredictiveText | additionalInputMethodHints

    Keys.onReturnPressed: moveToNextInput()
    Keys.onEnterPressed: moveToNextInput()
    Keys.onTabPressed: moveToNextInput()


    background: Rectangle {
        radius: s(Style.smallMargin)
        border {
            width: sr(1)
            color: Style.inputBorderColor
        }
    }

    SvgImage {
        id: icon
        property real horizontalMargins: s(Style.normalMargin)

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left

            leftMargin: horizontalMargins
            rightMargin: horizontalMargins
            topMargin: top.padding
            bottomMargin: top.padding
        }
        width: height

        visible: status != Image.Null && showIcon
    }
}
