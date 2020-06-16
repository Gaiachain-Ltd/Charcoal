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
    property bool isTraditional: false
    property string ovenName
    property alias popupTitle: comboBox.popupTitle

    property alias checkIcon: comboBox.checkIcon
    property alias delegateTextColor: comboBox.delegateTextColor

    property alias multiSelect: comboBox.multiSelect

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

                    delegate: Item {
                        readonly property string text: translatedName
                        readonly property bool isTraditional: isTraditionalOven
                        readonly property string ovenName: name

                        id: delegateItem
                        width: entriesList.width
                        height: layout.implicitHeight

                        ColumnLayout {
                            id: layout
                            anchors.fill: parent
                            RowLayout {
                                Items.GText {
                                    Layout.fillWidth: true

                                    id: delegateText
                                    text: delegateItem.text
                                    color: delegateTextColor
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    font.bold: true
                                    padding: s(GStyle.middleMargin)
                                }

                                Image {
                                    Layout.rightMargin: s(GStyle.middleMargin)
                                    source: checkIcon
                                    visible: comboBox.selection.includes(text)
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
                                let newSelection = comboBox.selection

                                if (multiSelect === false) {
                                    newSelection = []
                                }

                                if (newSelection.includes(text)) {
                                    newSelection.splice(newSelection.indexOf(text), 1)
                                } else {
                                    newSelection.push(text)
                                    top.isTraditional = delegateItem.isTraditional
                                    top.ovenName = delegateItem.ovenName
                                }
                                newSelection.sort()
                                comboBox.selection = newSelection
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

                    onClicked: {
                        comboBox.text = ""
                        for (let index = 0; index < comboBox.selection.length; ++index) {
                            if (comboBox.text.length === 0) {
                                comboBox.text += comboBox.selection[index]
                            } else {
                                comboBox.text += ", " + comboBox.selection[index]
                            }
                        }

                        popup.close()
                    }
                }

                Components.Footer {
                    id: footer
                    Layout.fillWidth: true
                }

            }
        }
    }
}
