import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items"
import "dummy" as DummyComponents

Item {
    id: top

    readonly property bool isCocoa: mainController.flavor === "cocoa"

    function colorForState(state) {
        if (userManager.offlineMode) {
            return GStyle.unknownColor
        }

        switch (state) {
        case Enums.ConnectionState.ConnectionSuccessful:
            return GStyle.okColor
        case Enums.ConnectionState.Connecting:
            return GStyle.warningColor
        case Enums.ConnectionState.ConnectionError:
            return GStyle.errorColor
        default:
        }
        return GStyle.unknownColor
    }

    function textForState(state) {
        if (userManager.offlineMode) {
            return Strings.offline
        }

        switch (state) {
        case Enums.ConnectionState.ConnectionSuccessful:
            if (isCocoa) {
                return Strings.online
            } else {
                return Strings.onlineUserInfo.arg(userManager.userData.job)
                                             .arg(userManager.userData.email)
            }
        case Enums.ConnectionState.Connecting:
            return Strings.searching
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
        spacing: s(GStyle.bigMargin)

        NumberImageButton {
            Layout.alignment: Qt.AlignVCenter

            number: localEventsModel.size

            source: GStyle.userImgUrl
            palette.button: top.currentColor

            disabledColorTint: GStyle.blank
            enabled: localEventsModel.size > 0

            onClicked: pageManager.enter(Enums.Page.WaitingTransactions)
        }

        GText {
            Layout.fillHeight: true
            Layout.fillWidth: true

            text: top.textForState(top.connectionState)
            color: top.currentColor

            font.bold: true
            horizontalAlignment: Qt.AlignLeft

            DummyComponents.ServerStateChanger {}
        }
    }
}
