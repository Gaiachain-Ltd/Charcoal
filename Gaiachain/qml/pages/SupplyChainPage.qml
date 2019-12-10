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

    property string packageId
    property string packageCodeData
    property var action: Enums.SupplyChainAction.Unknown

    property alias pageContent: contentLayout.data
    property alias proceedButtonEnabled: proceedButton.enabled
    property alias proceedButtonText: proceedButton.text

    property string buttonText: Strings.proceed

    function proceed() { // redefined in supply chain pages

    }

    function backToHomeHandler() {
        pageManager.openPopup(Enums.Popup.Confirm, { "text": Strings.askForExit })
    }

    function isCurrentAction(packageId, codeData, action) {
        var currentPackage = (top.packageId !== "" &&
                (top.packageId === packageId)) ||
                top.packageId === "" && codeData &&
                (top.packageCodeData === codeData.toString());

        return currentPackage && (top.action === action);
    }

    function handleActionAdded(local = false) {
        hideOverlay()
        pageManager.backTo(pageManager.homePage())

        if (local) {
            pageManager.openPopup(Enums.Popup.Notification, {"text": Strings.offlineActionAdded, "backgroundColor": Style.warningColor})
        } else {
            pageManager.openPopup(Enums.Popup.Notification, {"text": Strings.success})
        }
    }
    function handleActionError(code) {
        hideOverlay()

        var errorText = Strings.addActionErrorUnknown
        if (RequestHelper.isNetworkError(code) || RequestHelper.isServerError(code)) {
            errorText = Strings.addActionErrorOffline
        } else if (RequestHelper.isActionMissingError(code)) {
            errorText = Strings.addActionErrorMissing
        } else if (RequestHelper.isActionDuplicatedError(code)) {
            errorText = Strings.addActionErrorDuplicated
        }

        pageManager.openPopup(Enums.Popup.Notification, {"text": errorText, "backgroundColor": Style.errorColor})
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
        target: dataManager

        onLocalActionAdded: {
            if (!isCurrentAction(packageId, {}, action)) {
                return
            }

            if (userManager.offlineMode) {
                handleActionAdded(true)
            }
        }

        onLocalActionDuplicated: {
            if (!isCurrentAction(packageId, {}, action)) {
                return
            }

            if (userManager.offlineMode) {
                handleActionError(RequestHelper.actionDuplicatedError())
            } else {
                addActionResponseHandler.locallyDuplicated = true
            }
        }
    }
    Connections {
        id: addActionResponseHandler
        target: sessionManager

        property var locallyDuplicated: false

        onEntitySaved: {
            if (!isCurrentAction(packageId, codeData, action)) {
                return
            }

            handleActionAdded()
        }

        onEntitySaveError: {
            if (!isCurrentAction(packageId, codeData, action)) {
                return
            }

            if (DataGlobals.availableOfflineActionsQml().includes(Number(action)) &&
                    (RequestHelper.isNetworkError(code) || RequestHelper.isServerError(code)) ) {
                if (locallyDuplicated) {
                    handleActionError(RequestHelper.actionDuplicatedError())
                } else {
                    handleActionAdded(true)
                }
            } else {
                handleActionError(code)
            }

            locallyDuplicated = false
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
