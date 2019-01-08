import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

// TO_DO consider using Toolbar
Item {
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

        backgroundColor: !addButton.isActive ? Style.buttonGreenColor : "transparent"
        source: !addButton.isActive ? Style.gaiaBlackImgUrl : Style.gaiaColorImgUrl
        fillMode: Image.PreserveAspectFit

        onClicked: {
            pageManager.goToInitial();
        }
    }

    ImageButton
    {
        id: addButton
        anchors.centerIn: parent

        readonly property bool isActive: page === Enums.Page.QRScanner

        backgroundColor: isActive ? Style.buttonGreenColor : "transparent"
        source: isActive ? Style.buttonAddBlack : Style.buttonAddGreen

        visible: page !== Enums.Page.Login
        fillMode: Image.PreserveAspectFit

        onClicked: {
            if (!isActive)
                pageManager.push(Enums.Page.QRScanner)
        }
    }
}
