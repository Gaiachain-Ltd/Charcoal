import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items
import "../components" as Components

Rectangle {
    id: top

    property alias textItem: statusText
    property alias text: statusText.text
    property alias textColor: statusText.color

    implicitHeight: s(Style.footerHeight)

    color: Style.backgroundColor

    Items.BasicText {
        id: statusText

        font {
            pixelSize: s(Style.buttonPixelSize)
            family: Style.primaryFontFamily
            capitalization: Font.AllUppercase
            letterSpacing: s(Style.buttonLetterSpacing)
            weight: Font.DemiBold
        }

        color: Style.textPrimaryColor

        anchors.centerIn: parent
    }
}

