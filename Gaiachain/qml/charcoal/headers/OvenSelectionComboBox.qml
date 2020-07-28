import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as CharcoalItems
import "../../headers" as Headers
import "../../items" as Items
import "../../components" as Components

Headers.GHeader {
    id: top

    property alias model: comboBox.model
    property alias placeholderText: comboBox.placeholderText
    property alias currentText: comboBox.text
    property alias popupTitle: comboBox.popupTitle

    property alias checkIcon: comboBox.checkIcon
    property alias delegateTextColor: comboBox.delegateTextColor

    property alias multiSelect: comboBox.multiSelect
    property alias selectedItems: comboBox.selection

    property var ids: []
    property var letters: []
    property var descriptions: []

    onSelectedItemsChanged: {
        ids = []
        letters = []
        descriptions = []
        for (let i = 0; i < selectedItems.length; i++) {
            let item = selectedItems[i]
            ids.push(item["ovenId"])
            letters.push(item["letterId"])
            descriptions.push(item["firstRow"])
        }
    }

    widget: Items.GInput {
        id: comboBox

        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        enabled: top.enabled

        property alias popupTitle: popup.title
        property alias model: entriesList.model

        property url checkIcon: GStyle.checkBlackUrl
        property color delegateTextColor: GStyle.textPrimaryColor

        property bool multiSelect: false
        property var selection: []

        readOnly: true
        iconSource: GStyle.downArrowImgUrl
        iconEdge: Enums.Edge.RightEdge
        showIcon: true
        placeholderText: Strings.toSelect

        MouseArea {
            anchors.fill: parent
            onClicked: popup.open()
        }

        Popup {
            id: popup

            property bool showCloseButton: false
            property bool logoVisible: false

            property int currentIndex: -1

            property string title: Strings.selectDate

            anchors.centerIn: Overlay.overlay
            width: Overlay.overlay? Overlay.overlay.width : 100
            height: Overlay.overlay? Overlay.overlay.height : 100

            focus: true
            modal: true

            ColumnLayout {
                anchors.fill: parent

                spacing: 0

                Components.NavigationHeader {
                    id: header
                    Layout.fillWidth: true

                    title: popup.title
                    showCloseButton: popup.showCloseButton
                    showBackButton: true
                    enableBackButton: true
                    logoVisible: popup.logoVisible

                    // Reimplemented NavigationHeader.goBack()
                    function goBack() {
                        popup.close()
                    }
                }

                ListView {
                    id: entriesList
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    clip: true
                    currentIndex: popup.currentIndex

                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        readonly property string oven: ovenId
                        readonly property string text: letterId
                        readonly property string extraHeader: firstRow
                        readonly property string extraText: secondRow

                        id: delegateItem
                        width: entriesList.width
                        height: layout.implicitHeight
                        color: comboBox.text.includes(text)? "#eeeeee" : "transparent"

                        ColumnLayout {
                            id: layout
                            anchors.fill: parent
                            RowLayout {
                                Items.GText {
                                    id: delegateText
                                    text: delegateItem.text
                                    color: delegateTextColor
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    font.bold: true
                                    padding: s(GStyle.middleMargin)
                                }

                                ColumnLayout {
                                    id: extraInfo

                                    Items.GText {
                                        Layout.fillWidth: true
                                        text: delegateItem.extraHeader
                                        color: GStyle.textPrimaryColor
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignLeft
                                        font.bold: true
                                        font.pixelSize: s(GStyle.tinyPixelSize)
                                    }

                                    Items.GText {
                                        Layout.fillWidth: true
                                        text: delegateItem.extraText
                                        color: GStyle.textPrimaryColor
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignLeft
                                        font.bold: false
                                        font.pixelSize: s(GStyle.tinyPixelSize)
                                    }
                                }

                                Image {
                                    Layout.rightMargin: s(GStyle.middleMargin)
                                    source: checkIcon
                                    visible: comboBox.text.includes(text)
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true

                                color: GStyle.separatorColor
                                height: 1
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                let newSelection = []

                                if (multiSelect) {
                                    let isNew = true
                                    for (let i = 0; i < comboBox.selection.length; i++) {
                                        let item = comboBox.selection[i]
                                        if (delegateItem.oven === item["ovenId"]) {
                                            isNew = false
                                            continue
                                        } else {
                                            console.log("Pushing existing item", JSON.stringify(item))
                                            newSelection.push(item)
                                        }
                                    }

                                    if (isNew) {
                                        console.log("Pushing new selection", oven, letterId)
                                        newSelection.push(
                                                    {
                                                        "ovenId": oven,
                                                        "letterId": letterId,
                                                        "firstRow": extraHeader,
                                                        "secondRow": extraText
                                                    })
                                    }
                                } else {
                                    newSelection.push(
                                                {
                                                    "ovenId": oven,
                                                    "letterId": letterId,
                                                    "firstRow": extraHeader,
                                                    "secondRow": extraText
                                                })
                                }

                                comboBox.selection = newSelection

                                // Update text displayed when popup is closed
                                comboBox.text = ""
                                for (let index = 0; index < comboBox.selection.length; index++) {
                                    if (comboBox.text.length === 0) {
                                        comboBox.text += comboBox.selection[index]["letterId"]
                                    } else {
                                        comboBox.text += ", " + comboBox.selection[index]["letterId"]
                                    }
                                }
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar {}
                }

                Items.GButton {
                    id: selectButton

                    Layout.leftMargin: s(GStyle.hugeMargin)
                    Layout.rightMargin: Layout.leftMargin
                    Layout.bottomMargin: Layout.leftMargin
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom

                    padding: s(GStyle.bigMargin)

                    enabled: true

                    text: Strings.select

                    onClicked: popup.close()
                }

                Components.Footer {
                    id: footer
                    Layout.fillWidth: true
                }

            }
        }
    }
}
