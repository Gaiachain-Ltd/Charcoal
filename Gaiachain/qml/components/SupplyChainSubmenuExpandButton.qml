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
        spacing: Style.none

        MenuButton {
            id: menuButton

            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.submenuButtonHeight)
            Layout.bottomMargin: !categorySubmenu.empty && top.opened ? -1 * s(Style.smallMargin) : Style.none

            z: categorySubmenu.z + 1

            palette.button: top.opened ? Style.expandButtonSubmenuColor : Style.buttonMenuColor

            padding: s(Style.middleMargin)
            spacing: s(Style.middleMargin)

            icon.height: s(Style.submenuButtonImageHeight)
            icon.width: s(Style.submenuButtonImageHeight)

            font.pixelSize: s(Style.submenuButtonPixelSize)

            onClicked: opened = !opened

            Items.SvgImage {
                width: s(Style.submenuButtonImageHeight)
                height: s(Style.submenuButtonImageHeight)

                anchors {
                    top: parent.top
                    topMargin: s(Style.bigMargin)
                    bottom: parent.bottom
                    bottomMargin: s(Style.bigMargin)

                    right: parent.right
                    rightMargin: s(Style.bigMargin)
                }

                source: top.opened ? Style.upArrowImgUrl : Style.downArrowImgUrl
            }
        }

        Components.SupplyChainCategorySubmenu {
            id: categorySubmenu

            Layout.fillWidth: true
            Layout.preferredHeight: top.opened ? contentHeight : Style.none

            opacity: top.opened ? 1 : 0

            onClicked: mainLayout.parent.clicked(actionType)
        }
    }
}
