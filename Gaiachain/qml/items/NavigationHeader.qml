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

    signal headerClicked()

    function backHandler() {
        pageManager.back()
        return false    // do not close application
    }
    function logout() {
        pageManager.backTo(Enums.Page.Login)
        userManager.logOut()
    }

    property string title
    property bool logoVisible: false

    implicitHeight: s(Style.headerHeight)

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.headerBackgroundColor
    }

    RowLayout {
        anchors {
            fill: parent
            leftMargin: s(Style.normalMargin)
            rightMargin: s(Style.normalMargin)
            topMargin: s(Style.smallMargin)
            bottomMargin: s(Style.smallMargin)
        }

        PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)

            opacity: isOnHomePage || isBackToHomePage ? 0 : 1
            enabled: !isOnHomePage && !isBackToHomePage
            source: Style.backImgUrl

            onClicked: backHandler()
        }

        Items.LayoutSpacer { visible: !logoVisible }

        PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.buttonImageBigHeight)

            source: Style.logoWhiteImgUrl
            visible: logoVisible

            onClicked: headerClicked()
        }

        BasicText {
            text: title
            color: Style.textSecondaryColor
            font.pixelSize: s(Style.titlePixelSize)

            visible: !logoVisible
        }

        Items.LayoutSpacer { visible: !logoVisible }

        PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)

            source: Style.closeImgUrl
            visible: !isOnHomePage

            onClicked: pageManager.backTo(pageManager.homePage())
        }
        PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)

            source: Style.logoutImgUrl
            visible: isOnHomePage

            onClicked: logout()
        }
    }
}
