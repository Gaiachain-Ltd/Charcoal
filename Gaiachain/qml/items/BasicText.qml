import QtQuick 2.11
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

Label {
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    color: Style.textPrimaryColor
    font {
        pixelSize: s(Style.pixelSize)
        family: Style.primaryFontFamily
    }

    elide: Qt.ElideRight
}
