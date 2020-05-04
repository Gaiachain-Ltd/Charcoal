import QtQuick 2.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

QtObject
{
    function showAddActionError(packageId, action, error) {
        var errorText = error;
        errorText += "\n\n" + packageId
        errorText += "\n" + Helper.actionDescriptionStatusText(action)

        pageManager.openPopup(Enums.Popup.Information, {"text": errorText, "buttonPrimaryColor": GStyle.errorColor})
    }

    property var sessionManagerConnections: Connections {
        target: sessionManager
        enabled: (Number(pageManager.topPage) === page)

        // handle notification for package sent error (here for offline added actions)
        onEntitySaveError: {
            if ((page === Enums.SupplyChainSummary)
                    && supplyChainPage.isCurrentAction(packageId, codeData, action)) {
                return
            }

            // is offline added action
            if (RequestHelper.isOfflineError(code)) {
                return
            }

            var errorText = Strings.addActionErrorUnknown
            if (RequestHelper.isActionMissingError(code)) {
                errorText = Strings.addActionErrorMissing
            } else if (RequestHelper.isActionDuplicatedError(code)) {
                errorText = Strings.addActionErrorDuplicated
            }

            showAddActionError(packageId, action, errorText)
        }
    }

    property var dataManagerConnections: Connections {
        target: dataManager
        enabled: (Number(pageManager.topPage) === page)

        onLocalActionDataError: {
            var errorText = Strings.addActionErrorLocalData
            showAddActionError(packageId, action, errorText)
        }
    }

}
