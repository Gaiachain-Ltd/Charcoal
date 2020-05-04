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

    implicitHeight: s(GStyle.footerHeight)

    color: GStyle.backgroundColor

    RowLayout {
        anchors {
            fill: parent
            leftMargin: s(GStyle.hugeMargin)
            rightMargin: s(GStyle.hugeMargin)
        }

        spacing: s(GStyle.tinyMargin)

        Items.GText {
            id: statusText

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            font {
                pixelSize: s(GStyle.buttonPixelSize)
                family: GStyle.primaryFontFamily
                capitalization: Font.AllUppercase
                letterSpacing: s(GStyle.buttonLetterSpacing)
                weight: Font.DemiBold
            }

            color: GStyle.textPrimaryColor

            visible: !top.manual
        }

        Items.GInput {
            id: manualInput

            Layout.fillWidth: true

            inputMask: Static.qrInputMask

            focus: false
            visible: top.manual
        }

        Items.PureImageButton {
            id: imageButton

            Layout.alignment: Qt.AlignRight
            Layout.preferredHeight: s(GStyle.buttonImageMiddleBigHeight)
            Layout.preferredWidth: s(GStyle.buttonImageMiddleBigHeight)

            source: top.manual ? GStyle.enterImgUrl : GStyle.typeManuallyImgUrl

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
                to: !top.scanning ? GStyle.visible : GStyle.hidden
                duration: GStyle.animationDuration;
                easing.type: GStyle.animationEasing
            }
            PropertyAnimation {
                target: imageButton
                property: "Layout.preferredWidth"
                to: !top.scanning ? s(GStyle.buttonImageMiddleBigHeight) : GStyle.none
                duration: GStyle.animationDuration;
                easing.type: GStyle.animationEasing
            }
        }
    }
}

