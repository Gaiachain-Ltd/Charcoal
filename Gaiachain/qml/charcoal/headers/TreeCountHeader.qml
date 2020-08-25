import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../headers" as Headers

ColumnLayout {
    id: root

    Layout.fillWidth: true

    property alias plusHeaderText: button.headerText
    property alias plusButtonText: plusTextWidget.text
    property alias plusHelpText: button.helpText

    property alias counterHeaderText: counter.headerText
    readonly property alias counterValue: counterTextWidget.text
    property alias counterHelpText: counter.helpText

    property color secondaryTextColor
    property bool highlighted: false
    property bool extraLeftMargin: false
    property bool forceBoldValue: false
    property int valueFontSize: s(GStyle.inputPixelSize)
    property int margins: s(GStyle.bigMargin)

    Headers.GHeader {
        id: button

        Layout.fillWidth: true

        headerText: "Number of trees planted"
        helpButtonVisible: true
        helpText: "Trees number help"

        function clear() {
            text = Strings.empty
        }

        widget: Rectangle {
            id: plusRectangle

            Layout.fillWidth: true
            Layout.margins: root.margins
            Layout.leftMargin: extraLeftMargin? root.margins * 2 : root.margins

            color: GStyle.blank
            border {
                width: sr(GStyle.controlDefaultBorderWidth)
                color: GStyle.buttonBorderColor
            }

            height: s(350)
            radius: s(GStyle.buttonRadius)

            RowLayout {
                anchors.fill: parent
                anchors.margins: s(GStyle.middleMargin)
                spacing: s(GStyle.bigMargin)

                Text {
                    id: plusTextWidget
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    focus: false
                    color: secondaryTextColor
                    font.bold: root.highlighted || forceBoldValue
                    font.pixelSize: valueFontSize

                    text: "Tap to add a tree being planted"

                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                Image {
                    source: GStyle.iconPlus
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: counterTextWidget.text = parseInt(counterTextWidget.text) + 1
            }
        }
    }

    Headers.GHeader {
        id: counter

        Layout.fillWidth: true

        property alias text: counterTextWidget.text

        property bool forceBoldValue: false
        property int valueFontSize: s(GStyle.inputPixelSize)

        function clear() {
            text = Strings.empty
        }

        headerText: "Total count of trees planted"
        helpButtonVisible: true
        helpText: "Total trees number help"

        widget: Rectangle {
            id: inputRectangle

            Layout.fillWidth: true
            Layout.margins: root.margins
            Layout.leftMargin: extraLeftMargin? root.margins * 2 : root.margins

            color: GStyle.blank
            border {
                width: sr(GStyle.controlDefaultBorderWidth)
                color: GStyle.buttonBorderColor
            }

            height: s(100)
            radius: s(GStyle.buttonRadius)

            RowLayout {
                anchors.fill: parent

                TextField {
                    id: counterTextWidget
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    focus: false
                    color: secondaryTextColor
                    font.bold: root.highlighted || forceBoldValue
                    font.pixelSize: valueFontSize

                    text: "0"

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
                    Layout.maximumWidth: inputRectangle.height
                    width: inputRectangle.height

                    color: "#FBE0E0"
                    radius: s(GStyle.buttonRadius)

                    border {
                        width: sr(GStyle.controlDefaultBorderWidth)
                        color: GStyle.buttonBorderColor
                    }

                    //Image {
                    //    source: GStyle.iconMinus
                    //}

                    Rectangle {
                        anchors.centerIn: parent
                        color: "#D97F7F"
                        height: s(4)
                        radius: s(2)
                        width: parent.width * .4
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            let current = parseInt(counterTextWidget.text)
                            if (current > 0) {
                                counterTextWidget.text = current - 1
                            }
                        }
                    }
                }
            }
        }
    }
}
