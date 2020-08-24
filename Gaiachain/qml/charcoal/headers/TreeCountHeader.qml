import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../headers" as Headers

Column {
    id: root

    Layout.fillWidth: true

    property color secondaryColor
    property color secondaryTextColor
    property color backgroundColor
    property bool summaryMode: true
    property bool highlighted: false
    property bool extraLeftMargin: false
    property bool forceBoldValue: false
    property int valueFontSize: s(GStyle.inputPixelSize)
    property int margins: s(GStyle.bigMargin)

    Headers.GHeader {
        id: button

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

            height: 100
            radius: s(GStyle.buttonRadius)

            RowLayout {
                anchors.fill: parent

                Text {
                    id: plusTextWidget
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    focus: false
                    color: secondaryTextColor
                    font.bold: root.highlighted || forceBoldValue
                    font.pixelSize: valueFontSize

                    text: "Tap to add a tree being planted"

                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignLeft
                }

//                    Image {
//                    }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: counterTextWidget.text = parseInt(counterTextWidget.text) + 1
            }
        }
    }

    Headers.GHeader {
        id: counter

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

            height: 100
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

                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignLeft

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

//                    Image {
//                    }

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
