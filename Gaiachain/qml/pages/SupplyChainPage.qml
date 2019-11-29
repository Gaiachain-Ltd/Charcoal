import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components


BasePage {
    id: top

    property var action: Enums.SupplyChainAction.Unknown
    property alias pageContent: contentLayout.data
    property alias proceedButtonEnabled: proceedButton.enabled

    property string buttonText: Strings.proceed

    function proceed() { // redefined in supply chain pages

    }

    function backToHomeHandler() {
        pageManager.openPopup(Enums.Popup.Confirm, { "text": Strings.askForExit })
    }

    Connections {
        target: pageManager
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            switch (action) {
            case Enums.PopupAction.Accept:
                pageManager.backTo(pageManager.homePage())
                break
            default:
                break
            }
        }
    }

    Connections {
        target: sessionManager

        onEntitySaved: {
            pageManager.closePopup()
            pageManager.backTo(pageManager.homePage())
            pageManager.openPopup(Enums.Popup.Notification, {"text": Strings.success})
        }

        onEntitySaveError: {
            pageManager.closePopup()

            if ((RequestHelper.isNetworkError(code) || RequestHelper.isServerError(code)) &&
                    DataGlobals.availableOfflineActionsQml().includes(Number(action)) ) {
                pageManager.backTo(pageManager.homePage())
                pageManager.openPopup(Enums.Popup.Notification, {"text": Strings.offlineActionAdded, "backgroundColor": Style.warningColor})
            } else {
                pageManager.openPopup(Enums.Popup.Notification, {"text": Strings.addActionError, "backgroundColor": Style.errorColor})
            }
        }
    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: s(Style.hugeMargin)
        }

        spacing: s(Style.bigMargin)

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true

            contentHeight: contentLayout.implicitHeight

            clip: true

            boundsBehavior: Flickable.StopAtBounds

            ColumnLayout {
                id: contentLayout

                anchors.fill: parent
            }
        }

        Items.GenericButton {
            id: proceedButton

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            padding: s(Style.bigMargin)

            enabled: false

            text: Strings.proceed

            onClicked: top.proceed()
        }
    }
}
