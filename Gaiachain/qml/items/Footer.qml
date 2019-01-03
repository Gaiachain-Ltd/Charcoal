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
        anchors.centerIn: parent

        backgroundColor: page === Enums.Page.QrScanner ? Style.buttonGreenColor : "transparent"
        source: page === Enums.Page.QrScanner ? Style.buttonAddBlack : Style.buttonAddGreen
        fillMode: Image.PreserveAspectFit
        width: s(Style.buttonHeight)
        height: width

        onClicked: {
            if (page !== Enums.Page.QrScanner)
                pageManager.enterPage(Enums.Page.QrScanner)
        }

    }
}
