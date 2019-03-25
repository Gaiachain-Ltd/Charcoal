import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top
    headerVisible: false

    function enterViewTypePage(type) {
        userManager.commodityType = type
        pageManager.enter(Enums.Page.ViewType)
    }

    ColumnLayout {
        anchors.fill: parent

        // Custom page header
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.preferredHeight: s(Style.headerHeight)
            Layout.leftMargin: buttonLayout.margin
            readonly property int buttonHeight: s(Style.buttonHeight) * 0.9
            Layout.rightMargin: buttonHeight * 2
            Items.ImageButton {
                Layout.preferredHeight: parent.buttonHeight
                Layout.preferredWidth: parent.buttonHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                source: userManager.loggedIn ? Style.logoutImgUrl : Style.exitToLoginImgUrl

                onClicked: header.backHandler()
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Row {
                    anchors.centerIn: parent
                    spacing: 0
                    Items.SvgImage {
                        width: s(Style.buttonHeight)
                        height: s(Style.buttonHeight)
                        sourceSize.width: s(Style.buttonHeight)
                        sourceSize.height: s(Style.buttonHeight)
                        anchors.verticalCenter: parent.verticalCenter

                        source: Style.gaiaColorImgUrl
                    }

                    Items.ImageItem {
                        anchors.verticalCenter: parent.verticalCenter

                        text: Strings.gaiachain
                        textFont.pixelSize: s(Style.pixelSize)
                    }
                }
            }
        }

        RowLayout {
            id: buttonLayout
            Layout.fillWidth: true
            property real margin: s(Style.bigMargin) * 1.5
            property real buttonHeight: s(Style.buttonHeight) * 1.75
            Layout.preferredHeight: buttonHeight
            Layout.topMargin: margin
            Layout.leftMargin: margin

            spacing: margin

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width
                buttonPadding: s(Style.normalMargin) * 1.1
                buttonBackgroundColor: Style.buttonBackColor

                shadowHorizontalOffset: 0
                shadowVericalOffset: s(15)

                source: Style.chooseTimberImgUrl
                text: Strings.timber

                spacing: s(Style.normalMargin)

                onButtonClicked: top.enterViewTypePage(Enums.CommodityType.Timber)
            }

            Items.TextImageButton {
                Layout.preferredWidth: buttonLayout.buttonHeight

                buttonHeight: width
                buttonPadding: s(Style.normalMargin) * 1.1
                buttonBackgroundColor: Style.buttonBackBrownColor

                shadowHorizontalOffset: 0
                shadowVericalOffset: s(15)

                source: Style.chooseCocoaImgUrl
                text: Strings.cocoa

                spacing: s(Style.normalMargin)

//                onButtonClicked: top.enterViewTypePage(Enums.CommodityType.Cocoa)
            }
            Items.LayoutSpacer { spacerHeight: buttonLayout.buttonHeight }
        }

        Items.LayoutSpacer {}
    }
}
