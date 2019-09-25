import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

Item {
    id: top

    function colorForStatus(status) {
        switch (status) {
        case Enums.ConnectionStatus.ConnectionSuccessful:
            return Style.okColor
        case Enums.ConnectionStatus.Connecting:
            return Style.warningColor
        case Enums.ConnectionStatus.ConnectionError:
            return Style.errorColor
        default:
        }
        return Style.unknownColor
    }
    function textForStatus(status) {
        switch (status) {
        case Enums.ConnectionStatus.ConnectionSuccessful:
            return userManager.loggedIn ? userManager.login : Strings.anonymousUser
        case Enums.ConnectionStatus.Connecting:
            return Strings.searching + "..."
        case Enums.ConnectionStatus.ConnectionError:
            return Strings.offline
        default:
        }
        return ""
    }

    readonly property color currentColor: colorForStatus(connectionStatus)
    property var connectionStatus: Enums.ConnectionStatus.Unknown

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

            text: top.textForStatus(connectionStatus)
            color: top.currentColor

            font.bold: true
            horizontalAlignment: Qt.AlignLeft
        }
    }
}
