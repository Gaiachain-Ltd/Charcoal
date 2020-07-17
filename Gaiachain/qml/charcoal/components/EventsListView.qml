
import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../../items" as Items

ListView {
    id: root

    signal delegateClicked(string packageId, int type)

    spacing: s(GStyle.smallMargin)
    clip: true

    currentIndex: 0
    highlightRangeMode: ListView.StrictlyEnforceRange
    boundsBehavior: Flickable.StopAtBounds

    delegate: Item {
        width: root.width
        height: item.height

        MouseArea {
            anchors.fill: parent
            // use "model." because action resolves it wrongly
            onClicked: {
                let summaryTitle
                if (model.type === Enums.PackageType.Plot) {
                    summaryTitle = Strings.plotIdDetails
                } else if (model.type === Enums.PackageType.Harvest) {
                    summaryTitle = Strings.harvestIdDetails
                } else if (model.type === Enums.PackageType.Transport) {
                    summaryTitle = Strings.transportIdDetails
                }

                //console.log("Opening:", type, eventSummary)
                pageManager.enter(Enums.Page.SupplyChainSummary,
                                  {
                                      "title": summaryTitle,
                                      "proceedButtonVisible": false,
                                      "summary": model.eventSummary
                                  })
            }
        }

        Column {
            id: item
            width: parent.width

            spacing: s(GStyle.middleSmallMargin)

            Items.GText {
                horizontalAlignment: Text.AlignLeft
                width: parent.width
                height: contentHeight

                font.bold: true
                font.pixelSize: s(GStyle.tinyPixelSize)
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
                    spacing: s(GStyle.smallMargin)

                    RowLayout {
                        Items.GText {
                            Layout.fillWidth: true

                            horizontalAlignment: Text.AlignLeft
                            font.bold: true
                            font.pixelSize: s(GStyle.smallPixelSize)

                            text: modelData.eventName
                        }

                        Items.GText {
                            horizontalAlignment: Text.AlignRight
                            font.pixelSize: s(GStyle.smallPixelSize)

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
