import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top
    headerVisible: false

    function enterViewTypePage() {
        pageManager.enter(Enums.Page.ViewType)
    }

    ColumnLayout {
        anchors.fill: parent

        Items.LayoutSpacer { spacerHeight: s(20) }
        // Logo
        Items.ImageItem {
            Layout.preferredHeight: s(150)
            Layout.alignment: Qt.AlignHCenter

            buttonWidth: height

            imageUrl: Style.cocoaImgUrl
            text: Strings.gaiachain
            textFont.pixelSize: s(40)
        }

        RowLayout {
            id: buttonLayout
            Layout.fillWidth: true
            property real buttonHeight: s(150)
            Layout.preferredHeight: buttonHeight

            spacing: s(20)

            Items.LayoutSpacer { spacerHeight: buttonLayout.buttonHeight }
            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width

                source: Style.timberImgUrl
                text: Strings.timber
                textFont.pixelSize: s(30)

                onButtonClicked: {
                    // TO_DO Set timber user
                    top.enterViewTypePage()
                }
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width

                source: Style.charcoalImgUrl
                text: Strings.charcoal
                textFont.pixelSize: s(30)

                onButtonClicked: {
                    // TO_DO Set charcoal user
                    top.enterViewTypePage()
                }
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width

                source: Style.cocoaImgUrl
                text: Strings.cocoa
                textFont.pixelSize: s(30)

                onButtonClicked: {
                    // TO_DO Set cocoa user
                    top.enterViewTypePage()
                }
            }
            Items.LayoutSpacer { spacerHeight: buttonLayout.buttonHeight }
        }

        Items.LayoutSpacer {}
    }
}
