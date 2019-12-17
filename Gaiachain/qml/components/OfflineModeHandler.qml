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

        pageManager.openPopup(Enums.Popup.Information, {"text": errorText, "buttonPrimaryColor": Style.errorColor})
    }

    property var sessionManagerConnections: Connections {
        target: sessionManager
        enabled: (Number(pageManager.topPage) === page)

        function isSupplyChainPage() {
            return page === Enums.SupplyChainHarvest ||
                    page === Enums.SupplyChainGrainProcessing ||
                    page === Enums.SupplyChainSectionReception ||
                    page === Enums.SupplyChainBagging ||
                    page === Enums.SupplyChainAddHarvestId ||
                    page === Enums.SupplyChainLotCreation ||
                    page === Enums.SupplyChainWarehouseTransport ||
                    page === Enums.SupplyChainExportReception;
        }

        // handle notification for package sent error (here for offline added actions)
        onEntitySaveError: {
            if (isSupplyChainPage()
                    && isCurrentAction(packageId, codeData, action)) {
                return
            }

            // is offline added action
            if (RequestHelper.isNetworkError(code) || RequestHelper.isServerError(code)) {
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
