
import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../../items" as Items

ListView {
    id: top

    //property real delegateHeight: GStyle.listViewDelegateDefaultHeight

    signal delegateClicked(string packageId)

    spacing: s(GStyle.smallMargin)
    clip: true

    currentIndex: 0
    highlightRangeMode: ListView.StrictlyEnforceRange
    boundsBehavior: Flickable.StopAtBounds

    delegate: Item {

        width: parent.width
        height: item.height

        MouseArea {
            anchors.fill: parent
            // use model because action resolve wrongly
            onClicked: delegateClicked(model.title)
        }

        Column {
            id: item
            width: parent.width

            spacing: s(GStyle.tinyMargin)

            Items.GText {
                horizontalAlignment: Text.AlignLeft
                width: parent.width
                height: contentHeight

                font.bold: true
                wrapMode: TextInput.WrapAnywhere
                elide: Text.ElideNone
                maximumLineCount: 5

                text: title
                //color: Helper.packageTypeColor(DataGlobals.packageType(action))

                color: {
                    if (type === Enums.PackageType.Plot) {
                        return GStyle.fontHighlightColor
                    } else if (type === Enums.PackageType.Harvest) {
                        return GStyle.fontHighlightColor2
                    } else if (type === Enums.PackageType.Transport) {
                        return GStyle.fontHighlightColor3
                    }
                }
            }

            Repeater {
                model: rows

                ColumnLayout {
                    width: parent.width
                    spacing: 0

                    RowLayout {
                        Items.GText {
                            Layout.fillWidth: true

                            horizontalAlignment: Text.AlignLeft
                            font.bold: true

                            text: (index === 0)? titleFrom : titleTo
                        }

                        Items.GText {
                            horizontalAlignment: Text.AlignRight

                            text: (index === 0)? from : to
                            //text: Helper.formatDate(Number(value))
                        }
                    }

                    Items.LayoutSeparator {
                        Layout.fillWidth: true
                        implicitHeight: (index === (rows-1))? (sr(GStyle.separatorHeight) * 2)
                                                            : sr(GStyle.separatorHeight)
                        visible: true
                    }
                }
            }
        }

    }
}
