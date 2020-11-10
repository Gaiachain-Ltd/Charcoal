import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top

    readonly property bool isOnHomePage: pageManager.isOnHomePage()
    readonly property bool isBackToHomePage: pageManager.isBackToHomePage()

    property bool showCloseButton: true
    property bool showBackButton: !(isOnHomePage || isBackToHomePage)
    property bool enableBackButton: showBackButton

    function logout() {
        pageManager.backTo(Enums.Page.Login)
        userManager.logOut()
    }

    property string title
    property bool logoVisible: false

    implicitHeight: s(GStyle.headerHeight)

    /*
     * Reimplement in object for custom handling
     */
    function goBack() {
        backHandler()
    }

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

            opacity: top.showBackButton
            enabled: top.enableBackButton

            source: GStyle.backImgUrl

            onClicked: goBack()
        }

        Items.LayoutSpacer { visible: !logoVisible }

        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.preferredHeight: s(GStyle.buttonImageBigHeight)

            source: GStyle.logoMalebiWhiteUrl
            visible: logoVisible

            onClicked: {
                pageManager.openPopup(Enums.Popup.About)
            }
        }

        Items.GText {
            Layout.fillWidth: true
            text: title
            color: GStyle.textSecondaryColor
            font.pixelSize: (mainController.flavor === "cocoa")? s(GStyle.titlePixelSize)
                                                               : s(GStyle.subtitlePixelSize)
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

            source: (mainController.flavor === "cocoa")?
                        GStyle.logoutImgUrl : GStyle.onOffButton
            visible: isOnHomePage

            onClicked: logout()
        }
    }
}
