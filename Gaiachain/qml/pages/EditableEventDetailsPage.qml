import QtQuick 2.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

EventDetailsPage {
    editable: true

    Timer {
        id: dataRequestTimer
        interval: Style.requestOverlayInterval
        onTriggered: {
            sessionManager.getEntity()
            mainOverlayVisible = false
            pageManager.backTo(pageManager.homePage())
        }
    }

    Connections {
        target: pageManager
        // When using popup always add checking if I'm on top
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            switch(action) {
            case Enums.PopupAction.Save:
                sessionManager.putEntity(attributes.shipmentId, attributes.action)
                mainOverlayVisible = true
                break
            case Enums.PopupAction.Exit:
                pageManager.backTo(pageManager.homePage())
                break
            case Enums.PopupAction.Cancel:
            default:
            }
        }
    }

    Connections
    {
        target: sessionManager

        onEntitySaveResult: {
            if (id == attributes.shipmentId) {
                if (result) {
                    dataManager.clearModels()
                    dataRequestTimer.start()
                } else {
                    mainOverlayVisible = false
                    pageManager.enterPopup(Enums.Popup.Information, {
                                                                    "text" : Strings.dataSaveError,
                                                                    "rejectButtonText": Strings.close,
                                                                    "rejectButtonType": Enums.PopupAction.Error
                                           }, true)
                }
            }
        }
    }

    onEditClicked: pageManager.back()

    onCancelClicked: pageManager.enterPopup(Enums.Popup.Information, {
                                                "text" : Strings.exitWithoutSaveQuestion,
                                                "acceptButtonText": Strings.exit,
                                                "rejectButtonText": Strings.cancel,
                                                "acceptButtonType": Enums.PopupAction.Exit
                                            }, true)

    onAcceptClicked: pageManager.enterPopup(Enums.Popup.Information, {
                                                "text" : Strings.saveQuestion,
                                                "acceptButtonText": Strings.save,
                                                "rejectButtonText": Strings.cancel,
                                                "acceptButtonType": Enums.PopupAction.Save
                                            })
}
