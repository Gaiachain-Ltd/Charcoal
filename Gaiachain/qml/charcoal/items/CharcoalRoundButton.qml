import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

RoundButton {
    id: control

    height: 80
    width: height

    icon.width: 28

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
