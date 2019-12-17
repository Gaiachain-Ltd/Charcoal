import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../common"

SupplyChainPageBase {
    id: top

    property string packageId
    property string packageCodeData
    property var action: Enums.SupplyChainAction.Unknown

    proceedButtonEnabled: validPageData && gpsSource.validCoordinate

    function coordinate() {
        return gpsSource.coordinate ? gpsSource.coordinate : QtPositioning.coordinate()
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

    PositionSourceHandler {
        id: gpsSource
    }

    Items.ButtonInputHeader {
        Layout.fillWidth: true

        inputText: "GPS: " + (gpsSource.validCoordinate ? gpsSource.coordinate.latitude + " " + gpsSource.coordinate.longitude
                                                        : "invalid")
        onClicked: gpsSource.update()
    }
}
