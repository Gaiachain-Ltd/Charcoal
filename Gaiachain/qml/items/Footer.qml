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

        source:  Style.gaiaColorImgUrl
        fillMode: Image.PreserveAspectFit

        onClicked: {
            console.log("Not implemented yet!")
        }
    }

    ImageButton
    {
        anchors.centerIn: parent

        backgroundColor: page === Enums.Page.QRScanner ? Style.buttonGreenColor : "transparent"
        source: page === Enums.Page.QRScanner ? Style.buttonAddBlack : Style.buttonAddGreen
        fillMode: Image.PreserveAspectFit

        onClicked: {
            if (page !== Enums.Page.QRScanner)
                pageManager.enterPage(Enums.Page.QRScanner)
        }
    }
}
