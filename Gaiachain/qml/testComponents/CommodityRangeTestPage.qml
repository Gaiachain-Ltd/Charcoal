import QtQuick 2.11

import com.gaiachain.enums 1.0

Item {
    Column {
        anchors.fill: parent

        Item {
            height: parent.height * 0.2
            width: parent.width

            Rectangle {
                anchors.fill: parent
                color: "violet"
                opacity: 0.8
            }

            Text {
                anchors.centerIn: parent
                text: "Click me!"
                font.pixelSize: 50
                font.capitalization: Font.AllUppercase
            }

            MouseArea {
                id: ma
                anchors.fill: parent
                property var res: [Enums.CommodityType.Timber, Enums.CommodityType.Charcoal, Enums.CommodityType.Cocoa]
                property int currentIdx: 0
                property int prevIdx: 0

                signal commodityClicked()

                onClicked: {
                    commodityProxy.setCommodityType(res[prevIdx], false)
                    commodityProxy.setCommodityType(res[currentIdx])

                    commodityClicked()

                    prevIdx = currentIdx
                    ++currentIdx

                    if (currentIdx >= res.length)
                        currentIdx = 0

                    console.log("Current index", currentIdx)
                }
            }

            Component.onCompleted: {
                var startDate = new Date(2016, 1, 1);
                var endDate = new Date(2020, 1, 1);
                calendarRangeProxyModel.setDateTimeRange(startDate, endDate)
            }
        }

        Rectangle {
            id: rect
            height: parent.height * 0.8
            width: parent.width

            color: "blue"
            opacity: 0.8

            property string commoditiesNames: ""
            Connections {
                target: commodityProxy
                onCommodityTypeChanged: {
                    rect.commoditiesNames = ""
                    if (commodityProxy.commodityEnabled(Enums.CommodityType.Timber))
                        rect.commoditiesNames += " Timber "
                    if (commodityProxy.commodityEnabled(Enums.CommodityType.Charcoal))
                        rect.commoditiesNames += " Charcoal "
                    if (commodityProxy.commodityEnabled(Enums.CommodityType.Cocoa))
                        rect.commoditiesNames += " Cocoa "
                    console.log("Current commodities", rect.commoditiesNames)
                }
            }

            ListView {
                anchors.fill: parent

                model: calendarRangeProxyModel

                delegate: Rectangle {
                    width: parent.width
                    height: txt.height
                    color: "gray"

                    Text {
                        id: txt
                        anchors.centerIn: parent

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        text: company + rect.commoditiesNames
                    }
                }

                onCountChanged: {
                    console.log("Model count", count)
                }
            }
        }
    }
}
