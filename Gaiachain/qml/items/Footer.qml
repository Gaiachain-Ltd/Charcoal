import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

// TO_DO consider using Toolbar
Item {
    id: top
    readonly property bool isAddButtonActive : page === Enums.Page.QRScanner
    signal barClicked()

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.registerBarBackgroundColor
    }

    ImageButton
    {
        anchors {
            left: parent.left
            leftMargin: s(Style.bigMargin)
            verticalCenter: parent.verticalCenter
        }

        backgroundColor: !isAddButtonActive ? Style.buttonGreenColor : "transparent"
        source: !isAddButtonActive ? Style.gaiaBlackImgUrl : Style.gaiaColorImgUrl
        fillMode: Image.PreserveAspectFit

        onClicked: {
            if (isAddButtonActive)
                pageManager.backTo(pageManager.homePage());
        }
    }

    ImageButton
    {
        id: addButton
        anchors.centerIn: parent

        backgroundColor: isAddButtonActive ? Style.buttonGreenColor : "transparent"
        source: isAddButtonActive ? Style.buttonAddBlack : Style.buttonAddGreen

        visible: page !== Enums.Page.Login && userManager.loggedIn
        fillMode: Image.PreserveAspectFit

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
