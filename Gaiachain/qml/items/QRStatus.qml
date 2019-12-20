import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.static 1.0

import "../items" as Items
import "../components" as Components

Rectangle {
    id: top

    property alias textItem: statusText
    property alias text: statusText.text
    property alias textColor: statusText.color

    property bool scanning: true
    property bool manual: false

    signal typedManually(string manualId)

    implicitHeight: s(Style.footerHeight)

    color: Style.backgroundColor

    RowLayout {
        anchors {
            fill: parent
            leftMargin: s(Style.hugeMargin)
            rightMargin: s(Style.hugeMargin)
        }

        spacing: s(Style.tinyMargin)

        Items.BasicText {
            id: statusText

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            font {
                pixelSize: s(Style.buttonPixelSize)
                family: Style.primaryFontFamily
                capitalization: Font.AllUppercase
                letterSpacing: s(Style.buttonLetterSpacing)
                weight: Font.DemiBold
            }

            color: Style.textPrimaryColor

            visible: !top.manual
        }

        Items.GenericInput {
            id: manualInput

            Layout.fillWidth: true

            inputMask: Strings.qrInputMask

            focus: false
            visible: top.manual
        }

        Items.PureImageButton {
            id: imageButton

            Layout.alignment: Qt.AlignRight
            Layout.preferredHeight: s(Style.buttonImageMiddleBigHeight)
            Layout.preferredWidth: s(Style.buttonImageMiddleBigHeight)

            source: top.manual ? Style.enterImgUrl : Style.typeManuallyImgUrl

            function handleClick() {
                if (!top.manual) {
                    top.manual = true
                    manualInput.focus = true
                } else {
                    var manualText = manualInput.text
                    if (manualText) {
                        top.manual = false
                        top.typedManually(manualText)
                        imageButton.text = Strings.empty
                        manualInput.focus = false
                    }
                }
            }

            onClicked: handleClick()
        }
    }

    Behavior on scanning {
        ParallelAnimation {
            PropertyAnimation {
                target: imageButton
                property: "opacity"
                to: !top.scanning ? Style.visible : Style.hidden
                duration: Style.animationDuration;
                easing.type: Style.animationEasing
            }
            PropertyAnimation {
                target: imageButton
                property: "Layout.preferredWidth"
                to: !top.scanning ? s(Style.buttonImageMiddleBigHeight) : Style.none
                duration: Style.animationDuration;
                easing.type: Style.animationEasing
            }
        }
    }
}

