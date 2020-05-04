import QtQuick 2.11
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

Label {
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    color: GStyle.textPrimaryColor
    font {
        pixelSize: s(GStyle.pixelSize)
        family: GStyle.primaryFontFamily
    }

    elide: Qt.ElideRight
}
