import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../headers" as Headers

Headers.GHeader {
    id: root

    Layout.fillWidth: true

    readonly property alias counterValue: counterTextWidget.text
    property bool forceBoldValue: false
    property int valueFontSize: s(GStyle.inputPixelSize)

    highlighted: false
    extraLeftMargin: false

    headerText: Strings.totalCountOfTreesCut
    helpButtonVisible: true
    helpText: Strings.loggingEndingNumberOfTreesHelp

    function clear() {
        text = Strings.empty
    }

    widget: Rectangle {
        id: plusRectangle

        Layout.fillWidth: true
        Layout.rightMargin: root.margins
        Layout.leftMargin: extraLeftMargin? root.margins * 2 : root.margins

        color: GStyle.blank
        border {
            width: sr(GStyle.controlDefaultBorderWidth)
            color: GStyle.buttonBorderColor
        }

        height: s(150)
        radius: s(GStyle.buttonRadius)

        RowLayout {
            anchors.fill: parent
            anchors.margins: s(GStyle.tinyMargin)
            spacing: s(GStyle.bigMargin)

            TextField {
                id: counterTextWidget
                Layout.fillWidth: true
                Layout.fillHeight: true

                focus: false
                font.bold: root.highlighted || forceBoldValue
                font.pixelSize: valueFontSize

                text: "0"

                onTextChanged: {
                    if (text.length === 0) {
                        text = 0
                    }
                }

                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText

                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                validator: IntValidator {
                    bottom: 0
                    top: 32767
                }

                background: Item {
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.maximumWidth: height
                Layout.minimumWidth: height
                width: height

                color: GStyle.plusSignBackgroundColor
                radius: s(GStyle.buttonRadius)

                //border {
                //    width: sr(GStyle.controlDefaultBorderWidth)
                //    color: GStyle.buttonBorderColor
                //}

                Image {
                    source: GStyle.iconPlus
                    anchors.centerIn: parent
                    width: parent.width * .5
                    height: width
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        let current = parseInt(counterTextWidget.text, 10)

                        if (current === NaN) {
                            counterTextWidget.text = "0"
                        }

                        if (current >= 0) {
                            counterTextWidget.text = current + 1
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.maximumWidth: height
                Layout.minimumWidth: height
                width: height

                color: GStyle.minusSignBackgroundColor
                radius: s(GStyle.buttonRadius)

                //border {
                //    width: sr(GStyle.controlDefaultBorderWidth)
                //    color: GStyle.buttonBorderColor
                //}

                Rectangle {
                    anchors.centerIn: parent
                    color: GStyle.minusSignColor
                    height: s(4)
                    radius: s(2)
                    width: parent.width * .4
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        let current = parseInt(counterTextWidget.text, 10)

                        if (current === NaN) {
                            counterTextWidget.text = "0"
                        }

                        if (current > 0) {
                            counterTextWidget.text = current - 1
                        }
                    }
                }
            }
        }
    }
}
