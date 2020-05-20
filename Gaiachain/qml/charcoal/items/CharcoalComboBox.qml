import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../components" as Components

Items.GInput {
    id: top

    property alias popupTitle: popup.title
    property alias model: entriesList.model

    property url checkIcon: GStyle.checkBlackUrl
    property color delegateTextColor: GStyle.textPrimaryColor

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
                    property string text: delegateText.text
                    width: entriesList.width
                    height: layout.implicitHeight

                    ColumnLayout {
                        id: layout
                        anchors.fill: parent
                        RowLayout {
                            Items.GText {
                                id: delegateText
                                Layout.fillWidth: true
                                text: modelData
                                color: delegateTextColor
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                font.bold: true
                                padding: s(GStyle.middleMargin)
                            }

                            Image {
                                Layout.rightMargin: s(GStyle.middleMargin)
                                source: checkIcon
                                visible: entriesList.currentIndex === index
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
                        onClicked: entriesList.currentIndex = index
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
                    top.text = entriesList.currentItem.text
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
