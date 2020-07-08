import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../components" as Components
import "../items" as Items

ButtonPopup {
    id: top

    anchors.centerIn: Overlay.overlay
    buttons: []

    contentItem: Components.AboutGaia {}

    background: Rectangle {
        radius: s(GStyle.tinyMargin)

        Items.PureImageButton {
            anchors {
                right: parent.right
                rightMargin: s(GStyle.smallMargin)
                top: parent.top
                topMargin: s(GStyle.smallMargin)
            }

            source: GStyle.closeBlackImgUrl

            width: s(40)
            height: width

            onClicked: pageManager.closePopup()
        }
    }
}
