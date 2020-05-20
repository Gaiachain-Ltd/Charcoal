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

    property int borderWidth: sr(1)
    property color borderColor: GStyle.inputBorderColor
    property color backgroundColor: GStyle.backgroundColor
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
    padding: s(GStyle.bigMargin)
    leftPadding: (iconEdge === Enums.Edge.LeftEdge) && icon.visible ? icon.width + 2 * icon.horizontalMargins : padding
    rightPadding: (iconEdge === Enums.Edge.RightEdge) && icon.visible ? icon.width + 2 * icon.horizontalMargins : padding

    font {
        pixelSize: s(GStyle.inputPixelSize)
        family: GStyle.primaryFontFamily
    }
    placeholderTextColor: GStyle.textPrimaryColor

    echoMode: isPassword ? TextInput.Password : TextInput.Normal
    inputMethodHints: Qt.ImhNoPredictiveText | (isPassword ? Qt.ImhSensitiveData : Qt.ImhNone) | additionalInputMethodHints

    Keys.onReturnPressed: { accepted(); moveToNextInput() }
    Keys.onEnterPressed: { accepted(); moveToNextInput() }
    Keys.onTabPressed: moveToNextInput()

    background: Rectangle {
        id: backgroundRect
        color: enabled ? top.backgroundColor : GStyle.backgroundDisabledColor
        radius: s(GStyle.tinyMargin)
        border {
            width: top.borderWidth
            color: top.borderColor
        }
    }

    SvgImage {
        id: icon
        property real horizontalMargins: s(GStyle.middleMargin)

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
