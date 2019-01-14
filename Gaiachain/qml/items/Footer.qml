import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

// TO_DO consider using Toolbar
Item {
    id: top
    readonly property bool isAddButtonActive : page === Enums.Page.QRScanner || page === Enums.Page.EditableEventDetails
    signal barClicked()

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.registerBarBackgroundColor
    }

    Row {
        id: buttonRow

        readonly property real buttonWidth: s(Style.buttonHeight)

        anchors {
            leftMargin: buttonRow.spacing
            rightMargin: buttonRow.spacing
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        spacing: Math.round((top.width - (buttonWidth * Style.headerMaximumButtonCount)) / (Style.headerMaximumButtonCount + 1))

        ImageButton
        {
            width: buttonRow.buttonWidth
            height: buttonRow.buttonWidth

            backgroundColor: !isAddButtonActive ? Style.buttonGreenColor : "transparent"
            source: !isAddButtonActive ? Style.gaiaBlackImgUrl : Style.gaiaColorImgUrl

            onClicked: {
                if (isAddButtonActive)
                    pageManager.backTo(pageManager.homePage());
            }
        }

        Item {
            // button slot
            width: buttonRow.buttonWidth
            height: buttonRow.buttonWidth
        }

        Item {
            width: buttonRow.buttonWidth
            height: buttonRow.buttonWidth

            ImageButton
            {
                id: addButton
                anchors.fill: parent

                backgroundColor: isAddButtonActive ? Style.buttonGreenColor : "transparent"
                source: isAddButtonActive ? Style.buttonAddBlack : Style.buttonAddGreen

                visible: page !== Enums.Page.Login && userManager.loggedIn

                onClicked: {
                    if (!isAddButtonActive) {
                        pageManager.backToAndEnter(pageManager.homePage(),
                                                   Enums.Page.QRScanner,
                                                   {},
                                                   true)
                    }
                }
            }
        }

        Item {
            // button slot
            width: buttonRow.buttonWidth
            height: buttonRow.buttonWidth
        }

        ImageButton
        {
            width: buttonRow.buttonWidth
            height: buttonRow.buttonWidth

            source: Style.refreshImgUrl

            enabled: !refreshTimer.running
            visible: page === Enums.Page.ViewType

            onClicked: {
                mainOverlayVisible = true
                dataManager.clearModels()
                refreshTimer.start()
            }
        }
    }

    Timer {
        id: refreshTimer
        interval: Style.requestOverlayInterval
        onTriggered: {
            sessionManager.getEntity()
            mainOverlayVisible = false
        }
    }
}
