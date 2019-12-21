import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    property alias viewModel: listView.model
    property alias delegateHeight: listView.delegateHeight
    property alias delegateIcon: listView.delegateIcon

    property bool displayDate: true
    property bool displayLastItemSeparator: false

    signal delegateClicked(string packageId, int action)
    signal delegateIconClicked(string packageId, int action)

    ListView {
        id: listView

        anchors.fill: parent

        property real delegateHeight: Style.listViewDelegateDefaultHeight

        readonly property bool hasIcon: delegateIcon.toString().length != 0
        property url delegateIcon

        spacing: s(Style.smallMargin)
        clip: true

        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        boundsBehavior: Flickable.StopAtBounds

        delegate: Item {
            id: delegate

            width: parent.width
            height: s(listView.delegateHeight)

            readonly property bool isLast: ((ListView.view.count - 1) === index)
            readonly property bool hasIcon: ListView.view.hasIcon
            readonly property url delegateIcon: ListView.view.delegateIcon

            MouseArea {
                anchors.fill: parent
                onClicked: delegateClicked(model.packageId, model.action)   // use model because action resolve wronlgy
            }

            GridLayout {
                anchors.fill: parent

                columns: 3
                columnSpacing: s(Style.bigMargin)
                rowSpacing: s(Style.tinyMargin)

                // row 1
                Items.BasicText {
                    Layout.fillHeight: true

                    horizontalAlignment: Text.AlignLeft

                    font.bold: true
                    visible: displayDate

                    text: Helper.formatDate(Number(timestamp))
                }

                Items.BasicText {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.columnSpan: displayDate ? 1 : 2

                    horizontalAlignment: Text.AlignLeft

                    font.bold: true
                    color: Helper.packageTypeColor(DataGlobals.packageType(action))

                    text: packageId
                }

                Items.PureImageButton {
                    Layout.preferredHeight: s(Style.buttonImageSmallHeight)
                    Layout.preferredWidth: s(Style.buttonImageSmallHeight)
                    Layout.rightMargin: s(Style.bigMargin)
                    Layout.rowSpan: 2

                    visible: hasIcon
                    source: delegateIcon

                    onClicked: delegateIconClicked(model.packageId, model.action)   // use model because action resolve wronlgy
                }

                // row 2
                Items.BasicText {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.columnSpan: 2

                    horizontalAlignment: Text.AlignLeft

                    text: Helper.actionDescriptionStatusText(Number(action))
                }

                Items.LayoutSeparator {
                    Layout.fillWidth: true
                    Layout.columnSpan: 3

                    visible: !delegate.isLast || displayLastItemSeparator
                }
            }

        }
    }
}
