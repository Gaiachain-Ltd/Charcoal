import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

RoundButton {
    id: control

    height: s(80)
    width: height

    icon.height: s(40)
    icon.width: s(40)

    display: AbstractButton.IconOnly

    palette.button: GStyle.backgroundColor
    palette.buttonText: icon.source === GStyle.logoutImgUrl?
                            GStyle.textPrimaryColor : "transparent"

    Rectangle {
        color: "transparent"
        anchors.fill: parent
        radius: parent.radius
        border.width: 1
        border.color: GStyle.buttonBorderColor
    }
}
