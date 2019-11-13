import QtQuick 2.11
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../components" as Components

Rectangle {
    property alias model: listView.model

    readonly property int contentHeight: listView.contentHeight
    readonly property bool empty: (listView.count === 0)

    signal clicked(int actionType)

    color: Style.buttonMenuColor
    radius: s(Style.tinyMargin)
    border {
        color: Style.buttonBorderColor
        width: sr(Style.controlDefaultBorderWidth)
    }

    ListView {
        id: listView

        anchors.fill: parent

        interactive: false
        spacing: Style.none

        delegate: Components.SupplyChainCategorySubmenuButton {
            topPadding: (index === 0) ? s(Style.hugeMargin) : Style.none
            onClicked: listView.parent.clicked(actionType)

            text: actionName
        }
    }
}
