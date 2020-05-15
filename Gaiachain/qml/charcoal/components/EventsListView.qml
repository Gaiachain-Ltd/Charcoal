
import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../../items" as Items

ListView {
    id: top

    //property real delegateHeight: GStyle.listViewDelegateDefaultHeight

    signal delegateClicked(string packageId, int action)

    spacing: s(GStyle.smallMargin)
    clip: true

    currentIndex: 0
    highlightRangeMode: ListView.StrictlyEnforceRange
    boundsBehavior: Flickable.StopAtBounds

    delegate: Item {
        id: delegate

        width: top.width
        //height: s(top.delegateHeight)
        height: item.implicitHeight

        //readonly property bool isLast: ((ListView.view.count - 1) === index)
        //readonly property bool hasIcon: ListView.view.hasIcon
        //readonly property url delegateIcon: ListView.view.delegateIcon

        MouseArea {
            anchors.fill: parent
            onClicked: delegateClicked(model.packageId, model.action)   // use model because action resolve wronlgy
        }

        RowLayout {
            id: item
            //anchors.fill: parent

            spacing: s(GStyle.tinyMargin)

            Items.GText {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight

                horizontalAlignment: Text.AlignLeft

                font.bold: true

                text: title
                //color: Helper.packageTypeColor(DataGlobals.packageType(action))

                color: {
                    if (type === "plot") {
                        return GStyle.fontHighlightColor
                    } else if (type === "harvest") {
                        return GStyle.fontHighlightColor2
                    } else if (type === "transport") {
                        return GStyle.fontHighlightColor3
                    }
                }
            }

            Repeater {
                model: values.length

                RowLayout {
                    readonly property string key: values[index][0]
                    readonly property string value: values[index][1]

                    spacing: 0

                    ColumnLayout {
                        Items.GText {
                            Layout.fillWidth: true
                            Layout.preferredHeight: contentHeight

                            horizontalAlignment: Text.AlignLeft
                            font.bold: true

                            text: key
                        }

                        Items.GText {
                            Layout.fillWidth: true
                            Layout.preferredHeight: contentHeight

                            horizontalAlignment: Text.AlignRight

                            text: value
                            //text: Helper.formatDate(Number(value))
                        }
                    }

                    Items.LayoutSeparator {
                        Layout.fillWidth: true

                        visible: true
                    }
                }
            }

            Items.LayoutSeparator {
                Layout.fillWidth: true

                visible: true
            }
        }

    }
}
