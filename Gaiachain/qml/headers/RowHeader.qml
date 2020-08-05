import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.AbstractListHeader {
    id: top

    headerUnderlineVisible: false

    signal valueChanged()

    widget: RowLayout {
        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins
        spacing: 0

        Repeater {
            id: repeater
            property alias readOnly: top.readOnly
            property alias backgroundColor: top.backgroundColor

            model: titles.length

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0

                Items.GText {
                    readonly property int strikeMargin: 7

                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true

                    visible: summaryMode
                    text: titles[index]
                    color: mainColor
                    font.bold: highlighted? true : false
                    font.capitalization: summaryMode? Font.AllLowercase : Font.Capitalize
                    font.pixelSize: s(GStyle.smallPixelSize)
                    textFormat: Text.PlainText
                    height: font.pixelSize

                    onLineLaidOut: {
                        if (summaryMode) {
                            let widgetMiddle = width * .5
                            let lineMiddle = contentWidth * .5

                            leftBar.width = widgetMiddle - lineMiddle - (strikeMargin * 2)
                            rightBar.x = widgetMiddle + lineMiddle + strikeMargin
                            rightBar.width = width - rightBar.x
                        }
                    }

                    Rectangle {
                        id: leftBar
                        visible: summaryMode
                        height: 1.5
                        width: 1
                        color: GStyle.separatorColor
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: parent.strikeMargin
                    }

                    Rectangle {
                        id: rightBar
                        visible: summaryMode
                        height: 1.5
                        width: 1
                        color: GStyle.separatorColor
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: parent.strikeMargin
                    }
                }

                Items.GInput {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true

                    placeholderText: titles[index]
                    text: values[index]
                    onTextChanged: {
                        values[index] = text
                        valueChanged()
                    }
                    horizontalAlignment: TextInput.AlignHCenter

                    padding: summaryMode? s(GStyle.picoMargin) : s(GStyle.bigMargin)

                    readOnly: repeater.readOnly
                    borderWidth: summaryMode? 0 : sr(1)
                    focus: false
                    backgroundColor: repeater.backgroundColor
                    color: highlighted || summaryMode? secondaryTextColor : mainColor
                    font.bold: highlighted? true : false
                    validator: DoubleValidator {
                        bottom: 0.0
                        top: 1000.0
                        decimals: 2
                        notation: DoubleValidator.StandardNotation
                    }
                }
            }
        }
    }
}
