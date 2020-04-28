import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../components" as Components
import "../items" as Items

BasePopup {
    id: top

    anchors.centerIn: Overlay.overlay
    buttons: []

    contentItem: Components.AboutGaia {}

    background: Rectangle {
        radius: s(Style.tinyMargin)

        Items.PureImageButton {
            anchors {
                right: parent.right
                rightMargin: s(Style.smallMargin)
                top: parent.top
                topMargin: s(Style.smallMargin)
            }

            source: Style.clearImgUrl

            width: 64
            height: 64

            onClicked: pageManager.closePopup()
        }
    }
}
