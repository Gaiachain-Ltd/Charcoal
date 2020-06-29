import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../headers" as Headers

Headers.AbstractListHeader {
    id: top

    widget: ColumnLayout {
        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        Repeater {
            id: repeater
            property alias readOnly: top.readOnly
            property alias backgroundColor: top.backgroundColor

            model: titles.length

            RowLayout {
                Layout.bottomMargin: s(GStyle.bigMargin)

                ColumnLayout {
                    Items.GText {
                        Layout.alignment: Qt.AlignLeft

                        visible: summaryMode
                        text: titles[index]
                        color: mainColor
                        font.bold: highlighted? true : false
                        font.capitalization: Font.AllUppercase
                    }

                    Items.GInput {
                        id: inputHeight

                        Layout.fillWidth: true
                        horizontalAlignment: TextInput.AlignLeft
                        padding: 0

                        placeholderText: titles[index]
                        text: values[index]
                        onTextChanged: values[index] = text

                        readOnly: repeater.readOnly
                        borderWidth: summaryMode? 0 : sr(1)
                        focus: false
                        backgroundColor: repeater.backgroundColor
                        color: highlighted? secondaryTextColor : mainColor
                        font.bold: false
                        font.capitalization: Font.AllUppercase
                        validator: DoubleValidator {
                            bottom: 0.0
                        }
                    }
                }

                Image {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 44

                    fillMode: Image.Pad
                    source: icons[index]
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter

                    MouseArea {
                        property bool hasLink: links[index] !== Enums.Page.InvalidPage

                        enabled: hasLink
                        anchors.fill: parent
                        onClicked: {
                            console.log("Data:", links[index], linkDatas[index])
                            pageManager.enter(links[index], linkDatas[index])
                        }
                    }
                }
            }
        }
    }
}
