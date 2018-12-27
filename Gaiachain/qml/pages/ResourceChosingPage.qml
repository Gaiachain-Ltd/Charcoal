import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top
    headerVisible: false

    function enterViewTypePage() {
        pageManager.enterPage(Enums.Page.ViewType)
    }

    ColumnLayout {
        anchors.fill: parent

        // spacer
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: s(20)
        }

        Items.ImageItem {
            Layout.preferredHeight: s(150)
//            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            buttonWidth: height

            imageUrl: "qrc:/ui/cocoa"
            text: "Gaiachain"
            textFont.pixelSize: s(40)

            Rectangle {
                anchors.fill: parent
                color: "red"
                opacity: 0.4
            }
        }

        RowLayout {
            id: buttonLayout
            Layout.fillWidth: true
            property real buttonHeight: s(150)
            Layout.preferredHeight: buttonHeight

            spacing: s(20)

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: buttonLayout.buttonHeight
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width

                source: "qrc:/ui/timber"
                text: qsTr("Timber")
                textFont.pixelSize: s(30)

                onButtonClicked: {
                    // TO_DO Set timber user
                    top.enterViewTypePage()
                }

                Rectangle {
                    anchors.fill: parent
                    color: "green"
                    opacity: 0.5
                }
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width

                source: "qrc:/ui/charcoal"
                text: qsTr("Charcoal")
                textFont.pixelSize: s(30)

                onButtonClicked: {
                    // TO_DO Set charcoal user
                    top.enterViewTypePage()
                }
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width

                source: "qrc:/ui/cocoa"
                text: qsTr("Cocoa")
                textFont.pixelSize: s(30)

                onButtonClicked: {
                    // TO_DO Set cocoa user
                    top.enterViewTypePage()
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: buttonLayout.buttonHeight
            }
        }

        // spacer
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
