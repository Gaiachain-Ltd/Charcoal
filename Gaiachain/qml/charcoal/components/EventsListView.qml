
import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../../items" as Items

ListView {
    id: top

    signal delegateClicked(string packageId, int type)

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
            // use model because action resolves it wrongly
            onClicked: delegateClicked(model.name, model.type)
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

                text: name

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
                model: events

                ColumnLayout {
                    width: parent.width
                    spacing: 0

                    RowLayout {
                        Items.GText {
                            Layout.fillWidth: true

                            horizontalAlignment: Text.AlignLeft
                            font.bold: true

                            text: modelData.eventName
                        }

                        Items.GText {
                            horizontalAlignment: Text.AlignRight

                            text: modelData.date
                        }
                    }

                    Items.LayoutSeparator {
                        Layout.fillWidth: true
                        implicitHeight: (index === (events.length-1))?
                                            (sr(GStyle.separatorHeight) * 2)
                                          : sr(GStyle.separatorHeight)
                        visible: true
                    }
                }
            }
        }

    }
}
