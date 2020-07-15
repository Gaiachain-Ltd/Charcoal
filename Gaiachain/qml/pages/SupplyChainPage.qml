import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.platforms 1.0

import "../items" as Items
import "../headers" as Headers
import "../common"

SupplyChainPageBase {
    id: top

    property string packageId
    property string packageCodeData
    property var action: Enums.SupplyChainAction.Unknown

    readonly property alias gpsCoordinates: gpsCoordinatesButtonInputHeader.inputText

    proceedButtonEnabled: validPageData && gpsSource.validCoordinate

    Component.onCompleted:  {
        if (typeof(AndroidPermissionsHandler) !== "undefined") {
            AndroidPermissionsHandler.requestPermission(AndroidPermissionsHandler.Location)
        }
    }

    function coordinate() {
        return gpsSource.coordinate ? gpsSource.coordinate : QtPositioning.coordinate()
    }

    function isCurrentAction(packageId, codeData, action) {
        var currentPackage = (top.packageId !== "" &&
                (top.packageId === packageId)) ||
                top.packageId === "" && codeData.toString().length &&
                (top.packageCodeData === codeData.toString());

        return currentPackage && (top.action === action);
    }

    function handleActionAdded(local = false) {
        hideOverlay()

        if (local) {
            pageManager.backToAndOpenPopup(pageManager.homePage(), Enums.Popup.Notification, {},
                                           {"text": Strings.offlineActionAdded, "backgroundColor": GStyle.warningColor,
                                               "iconSource": GStyle.warningImgUrl, "openedInterval": GStyle.notificationPopupOpenedLongInterval})
        } else {
            pageManager.backToAndOpenPopup(pageManager.homePage(), Enums.Popup.Notification, {},
                                           {"text": Strings.success})
        }
    }
    function handleActionError(code) {
        hideOverlay()

        var errorText = Strings.addActionErrorUnknown
        if (RequestHelper.isOfflineError(code)) {
            errorText = Strings.addActionErrorOffline
        } else if (RequestHelper.isActionMissingError(code)) {
            errorText = Strings.addActionErrorMissing
        } else if (RequestHelper.isActionDuplicatedError(code)) {
            errorText = Strings.addActionErrorDuplicated
        }

        pageManager.openPopup(Enums.Popup.Notification, {"text": errorText, "backgroundColor": GStyle.errorColor, "openedInterval": GStyle.notificationPopupOpenedLongInterval})
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
                    RequestHelper.isOfflineError(code)) {
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

        function errorMessage() {
            if (noAccess) {
                return Strings.enableGpsLocation
            } else if (!valid) {
                return Strings.gpsNotAvailable
            } else if (!positioningSupported) {
                return Strings.gpsTurnedOff
            } else {
                return Strings.gpsInvalid
            }
        }
    }

    Headers.ButtonInputHeader {
        id: gpsCoordinatesButtonInputHeader

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinateString) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
