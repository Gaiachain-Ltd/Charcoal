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
            Layout.fillWidth: true
            Layout.preferredHeight: s(150)
            imageUrl: "qrc:/ui/cocoa"
            imageText: "Gaiachain"
        }

        RowLayout {
            id: buttonLayout
            Layout.fillWidth: true
            property real preferredH: s(200)
            Layout.preferredHeight: preferredH

            spacing: s(20)

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: buttonLayout.preferredH
            }

            Items.TextImageButton {
                Layout.preferredHeight: buttonLayout.preferredH
                Layout.preferredWidth: Layout.preferredHeight
                buttonSource: "qrc:/ui/timber"
                bottomText: qsTr("Timber")
                onButtonClicked: {
                    // TO_DO Set timber user
                    top.enterViewTypePage()
                }
            }

            Items.TextImageButton {
                Layout.preferredHeight: buttonLayout.preferredH
                Layout.preferredWidth: Layout.preferredHeight
                buttonSource: "qrc:/ui/charcoal"
                bottomText: qsTr("Charcoal")
                onButtonClicked: {
                    // TO_DO Set charcoal user
                    top.enterViewTypePage()
                }
            }

            Items.TextImageButton {
                Layout.preferredHeight: buttonLayout.preferredH
                Layout.preferredWidth: Layout.preferredHeight
                buttonSource: "qrc:/ui/cocoa"
                bottomText: qsTr("Cocoa")
                onButtonClicked: {
                    // TO_DO Set cocoa user
                    top.enterViewTypePage()
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: buttonLayout.preferredH
            }
        }

        // spacer
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
