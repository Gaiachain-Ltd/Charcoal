import QtQuick 2.11

import com.gaiachain.style 1.0

Text {
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    color: Style.textPrimaryColor
    font {
        pixelSize: s(Style.pixelSize)
        family: Style.primaryFontFamily
    }

    elide: Qt.ElideRight
}
