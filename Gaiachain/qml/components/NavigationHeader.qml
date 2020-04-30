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

    function logout() {
        pageManager.backTo(Enums.Page.Login)
        userManager.logOut()
    }

    property string title
    property bool logoVisible: false

    implicitHeight: s(Style.headerHeight)

    Items.BlockMouseArea{}

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.headerBackgroundColor
    }

    RowLayout {
        anchors {
            fill: parent
            leftMargin: s(Style.middleMargin)
            rightMargin: s(Style.middleMargin)
            topMargin: s(Style.smallMargin)
            bottomMargin: s(Style.smallMargin)
        }

        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)

            opacity: isOnHomePage || isBackToHomePage ? 0 : 1
            enabled: !isOnHomePage && !isBackToHomePage
            source: Style.backImgUrl

            onClicked: backHandler()
        }

        Items.LayoutSpacer { visible: !logoVisible }

        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.buttonImageBigHeight)

            source: Style.logoWhiteImgUrl
            visible: logoVisible

            onClicked: {
                var message = Strings.aboutApp.arg(mainController.application.name)
                                              .arg(mainController.application.version)
                                              .arg(mainController.application.commit)
                                              .arg(mainController.application.domain);
                if (userManager.loggedIn) {
                    message += "<br/><br/>" + Strings.userInfo.arg(userManager.login)
                }

                pageManager.openPopup(Enums.Popup.Information, { "text": message, "textFormat": Text.StyledText })
            }
        }

        Items.BasicText {
            Layout.fillWidth: true
            text: title
            color: Style.textSecondaryColor
            font.pixelSize: s(Style.titlePixelSize)
            wrapMode: Text.WordWrap

            visible: !logoVisible
        }

        Items.LayoutSpacer { visible: !logoVisible }

        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)

            source: Style.closeImgUrl
            visible: !isOnHomePage

            onClicked: backToHomeHandler()
        }
        Items.PureImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)

            source: Style.logoutImgUrl
            visible: isOnHomePage

            onClicked: logout()
        }
    }
}
