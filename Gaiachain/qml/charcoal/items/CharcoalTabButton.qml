import QtQuick 2.12
import QtQuick.Controls 2.12

import "../../items" as Items

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

Items.GTabButton {
    property int packageTypes: Enums.PackageType.Plot | Enums.PackageType.Harvest | Enums.PackageType.Transport

    selectedColor: GStyle.headerBackgroundColor
    deselectedColor: GStyle.backgroundColor

    selectedFontColor: GStyle.backgroundColor
    deselectedFontColor: GStyle.textTabColor

    borderColor: GStyle.inputBorderColor
    borderWidth: sr(1)

    radius: s(5)

    font.capitalization: Font.AllUppercase
    font.bold: true
}
