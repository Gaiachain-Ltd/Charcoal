import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

// TO_DO consider using Toolbar
Item {
    id: top

    implicitHeight: s(Style.footerHeight)

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.footerBackgroundColor
    }

    RowLayout {
        id: topRow
        anchors {
            fill: parent
            margins: s(Style.normalMargin)
        }

        readonly property int buttonHeight: height * 0.9

        ImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: topRow.buttonHeight
            Layout.preferredWidth: Layout.preferredHeight

            source: Style.userImgUrl
        }
        BasicText {
            text: userManager.loggedIn ? userManager.login : Strings.anonymousUser
            color: Style.okColor
            font.bold: true
        }

        Item    // spacer
        {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
        }

        ImageButton {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: topRow.buttonHeight
            Layout.preferredWidth: Layout.preferredHeight

            source: Style.refreshImgUrl

            onClicked: refreshData()
        }
    }
}
