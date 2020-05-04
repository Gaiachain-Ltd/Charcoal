import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

Item {
    id: top

    property alias model: categorySubmenu.model
    property alias iconSource: menuButton.icon.source
    property alias text: menuButton.text

    property bool opened: false

    signal clicked(int actionType)

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: GStyle.none

        MenuButton {
            id: menuButton

            Layout.fillWidth: true
            Layout.preferredHeight: s(GStyle.submenuButtonHeight)
            Layout.bottomMargin: !categorySubmenu.empty && top.opened ? -1 * s(GStyle.smallMargin) : GStyle.none

            z: categorySubmenu.z + 1

            palette.button: top.opened ? GStyle.expandButtonSubmenuColor : GStyle.buttonMenuColor

            padding: s(GStyle.middleMargin)
            spacing: s(GStyle.middleMargin)

            icon.height: s(GStyle.submenuButtonImageHeight)
            icon.width: s(GStyle.submenuButtonImageHeight)

            font.pixelSize: s(GStyle.submenuButtonPixelSize)

            onClicked: opened = !opened

            Items.SvgImage {
                width: s(GStyle.submenuButtonImageHeight)
                height: s(GStyle.submenuButtonImageHeight)

                anchors {
                    top: parent.top
                    topMargin: s(GStyle.bigMargin)
                    bottom: parent.bottom
                    bottomMargin: s(GStyle.bigMargin)

                    right: parent.right
                    rightMargin: s(GStyle.bigMargin)
                }

                source: top.opened ? GStyle.upArrowImgUrl : GStyle.downArrowImgUrl
            }
        }

        Components.SupplyChainCategorySubmenu {
            id: categorySubmenu

            Layout.fillWidth: true
            Layout.preferredHeight: top.opened ? contentHeight : GStyle.none

            opacity: top.opened ? 1 : 0

            onClicked: mainLayout.parent.clicked(actionType)
        }
    }
}
