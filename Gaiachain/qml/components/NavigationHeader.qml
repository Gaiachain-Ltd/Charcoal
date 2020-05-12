import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top

    property bool showCloseButton: true
    readonly property bool isOnHomePage: pageManager.isOnHomePage()
    readonly property bool isBackToHomePage: pageManager.isBackToHomePage()

    function logout() {
        pageManager.backTo(Enums.Page.Login)
        userManager.logOut()
    }

    property string title
    property bool logoVisible: false

    implicitHeight: s(GStyle.headerHeight)

    Items.BlockMouseArea{}

    Rectangle {
        id: background
        anchors.fill: parent
        color: GStyle.headerBackgroundColor
    }

    RowLayout {
        anchors {
            fill: parent
            leftMargin: s(GStyle.middleMargin)
            rightMargin: s(GStyle.middleMargin)
            topMargin: s(GStyle.smallMargin)
            bottomMargin: s(GStyle.smallMargin)
        }

        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)

            opacity: isOnHomePage || isBackToHomePage ? 0 : 1
            enabled: !isOnHomePage && !isBackToHomePage
            source: GStyle.backImgUrl

            onClicked: backHandler()
        }

        Items.LayoutSpacer { visible: !logoVisible }

        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.preferredHeight: s(GStyle.buttonImageBigHeight)

            source: GStyle.logoWhiteImgUrl
            visible: logoVisible

            onClicked: {
                pageManager.openPopup(Enums.Popup.About)
            }
        }

        Items.GText {
            Layout.fillWidth: true
            text: title
            color: GStyle.textSecondaryColor
            font.pixelSize: s(GStyle.titlePixelSize)
            font.capitalization: GStyle.headerCapitalization
            wrapMode: Text.WordWrap

            visible: !logoVisible
        }

        Items.LayoutSpacer { visible: !logoVisible }

        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)

            source: GStyle.closeImgUrl
            visible: !isOnHomePage && showCloseButton

            onClicked: backToHomeHandler()
        }
        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)

            source: GStyle.logoutImgUrl
            visible: isOnHomePage

            onClicked: logout()
        }
    }
}
