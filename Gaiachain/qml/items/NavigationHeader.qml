import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top

    readonly property bool isOnHomePage: pageManager.isOnHomePage()
    signal headerClicked()

    function backHandler() {
        if (!isOnHomePage) {
            pageManager.back()
        }
    }
    function logout() {
        userManager.logOut()
        pageManager.backTo(Enums.Page.Login) //TO_DO immediate generates bug
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
        id: topRow
        anchors {
            fill: parent
            leftMargin: s(Style.normalMargin)
            rightMargin: s(Style.normalMargin)
            topMargin: s(Style.smallMargin)
            bottomMargin: s(Style.smallMargin)
        }

        readonly property real logoHeight: height
        readonly property int buttonHeight: height * 0.75

        ImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: topRow.buttonHeight
            Layout.preferredWidth: Layout.preferredHeight

            opacity: isOnHomePage ? 0 : 1
            enabled: !isOnHomePage
            source: Style.backImgUrl

            onClicked: backHandler()
        }

        Items.LayoutSpacer {}

        ImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: topRow.logoHeight
            Layout.preferredWidth: Layout.preferredHeight

            source: Style.logoIconImgUrl
            visible: logoVisible

            onClicked: headerClicked()
        }

        BasicText {
            text: title
            color: Style.textSecondaryColor
            font.pixelSize: s(Style.titlePixelSize)
        }

        Items.LayoutSpacer {}

        ImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: topRow.buttonHeight
            Layout.preferredWidth: Layout.preferredHeight

            source: Style.logoutImgUrl

            onClicked: logout()
        }
    }
}
