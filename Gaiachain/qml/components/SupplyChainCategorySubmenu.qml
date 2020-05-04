import QtQuick 2.11
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../components" as Components

Rectangle {
    property alias model: listView.model

    readonly property int contentHeight: listView.contentHeight
    readonly property bool empty: (listView.count === 0)

    signal clicked(int actionType)

    color: GStyle.buttonMenuColor
    radius: s(GStyle.tinyMargin)
    border {
        color: GStyle.buttonBorderColor
        width: sr(GStyle.controlDefaultBorderWidth)
    }

    ListView {
        id: listView

        anchors.fill: parent

        interactive: false
        spacing: GStyle.none

        delegate: Components.SupplyChainCategorySubmenuButton {
            width: listView.width

            topPadding: (index === 0) ? s(GStyle.hugeMargin) : GStyle.none
            onClicked: listView.parent.clicked(actionType)

            text: actionName
        }
    }
}
