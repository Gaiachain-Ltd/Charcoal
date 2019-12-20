import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

TextField
{
    id: top

    property bool isPassword: false
    property int additionalInputMethodHints: Qt.ImhNone

    property alias iconSource: icon.source
    property var iconEdge: Enums.Edge.LeftEdge
    property bool showIcon: true

    property color borderColor: Style.inputBorderColor
    property color backgroundColor: Style.backgroundColor
    property alias iconItem: icon

    property Item nextInput: null
    signal moveToNextInput()
    signal iconClicked

    onMoveToNextInput: {
        if (nextInput !== null) {
            nextInput.focus = true
        } else {
            focus = false
            Qt.inputMethod.hide();
        }
    }

    focus: true
    autoScroll: true
    maximumLength: 50
    padding: s(Style.bigMargin)
    leftPadding: (iconEdge === Enums.Edge.LeftEdge) && icon.visible ? icon.width + 2 * icon.horizontalMargins : padding
    rightPadding: (iconEdge === Enums.Edge.RightEdge) && icon.visible ? icon.width + 2 * icon.horizontalMargins : padding

    font {
        pixelSize: s(Style.inputPixelSize)
        family: Style.primaryFontFamily
    }
    placeholderTextColor: Style.textPrimaryColor

    echoMode: isPassword ? TextInput.Password : TextInput.Normal
    inputMethodHints: Qt.ImhNoPredictiveText | (isPassword ? Qt.ImhSensitiveData : Qt.ImhNone) | additionalInputMethodHints

    Keys.onReturnPressed: { accepted(); moveToNextInput() }
    Keys.onEnterPressed: { accepted(); moveToNextInput() }
    Keys.onTabPressed: moveToNextInput()

    background: Rectangle {
        color: enabled ? top.backgroundColor : Style.backgroundDisabledColor
        radius: s(Style.tinyMargin)
        border {
            width: sr(1)
            color: top.borderColor
        }
    }

    SvgImage {
        id: icon
        property real horizontalMargins: s(Style.middleMargin)

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: (iconEdge == Enums.Edge.LeftEdge) ? parent.left : undefined
            right: (iconEdge == Enums.Edge.RightEdge) ? parent.right : undefined

            leftMargin: horizontalMargins
            rightMargin: horizontalMargins
            topMargin: top.padding
            bottomMargin: top.padding
        }
        width: height

        visible: status != Image.Null && showIcon

        MouseArea {
            anchors.fill: parent
            onClicked: top.iconClicked()
        }
    }
}
