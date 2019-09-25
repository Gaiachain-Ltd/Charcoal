import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

Item {
    id: top

    function colorForState(state) {
        switch (state) {
        case Enums.ConnectionState.ConnectionSuccessful:
            return Style.okColor
        case Enums.ConnectionState.Connecting:
            return Style.warningColor
        case Enums.ConnectionState.ConnectionError:
            return Style.errorColor
        default:
        }
        return Style.unknownColor
    }
    function textForState(state) {
        switch (state) {
        case Enums.ConnectionState.ConnectionSuccessful:
            return userManager.loggedIn ? userManager.login : Strings.anonymousUser
        case Enums.ConnectionState.Connecting:
            return Strings.searching + "..."
        case Enums.ConnectionState.ConnectionError:
            return Strings.offline
        default:
        }
        return ""
    }

    readonly property color currentColor: colorForState(connectionState)
    property int connectionState: Enums.ConnectionState.Unknown

    RowLayout {
        anchors.fill: parent
        spacing: s(Style.bigMargin)

        ImageItem {
            Layout.alignment: Qt.AlignVCenter

            source: Style.userImgUrl
            backgroundColor: top.currentColor
        }

        BasicText {
            Layout.fillHeight: true
            Layout.fillWidth: true

            text: top.textForState(top.connectionState)
            color: top.currentColor

            font.bold: true
            horizontalAlignment: Qt.AlignLeft
        }
    }
}
