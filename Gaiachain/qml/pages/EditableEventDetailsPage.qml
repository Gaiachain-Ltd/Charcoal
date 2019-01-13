import QtQuick 2.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

EventDetailsPage {
    editable: true

    Connections {
        target: pageManager
        // When using popup always add checking if I'm on top
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            switch(action) {
            case Enums.PopupAction.Save:
                console.log("Save action to implement")
                break
            case Enums.PopupAction.Exit:
                pageManager.backTo(pageManager.homePage())
                break
            case Enums.PopupAction.Cancel:
            default:
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
