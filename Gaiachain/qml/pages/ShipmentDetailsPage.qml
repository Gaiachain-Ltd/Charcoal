import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

BasePage {
    id: top

    property string shipmentId

    Component.onCompleted: shipmentEventsProxyModel.setShipmentId(shipmentId)
    Component.onDestruction: shipmentEventsProxyModel.clearShipmentId()

    Connections {
        target: shipmentEventsProxyModel
        onModelChanged: parseModelData()
    }

    ListModel {
        id: shipmentModel
    }

    function createDataObject(attributes) {
        // TODO find a better solution
        return { "company": attributes.company,
            "timestamp": Number(attributes.timestamp),
            "place": Number(attributes.place),
            "action": Number(attributes.action),
            "location": attributes.location,
            "shipmentId": attributes.shipmentId
          };
    }
    function addModelData(currentPlace, eventsList) {
        shipmentModel.append({ "place": Number(currentPlace), "events": eventsList})
    }
    function parseModelData() {
        shipmentModel.clear()

        var companyEventsList = [];
        var currentCompany = undefined;
        var currentPlace = undefined;

        for (var row = 0; row < shipmentEventsProxyModel.rowCount(); ++ row) {
            var rowAttributes = shipmentEventsProxyModel.getRowAttributes(row);
            if(currentCompany !== rowAttributes.company || currentPlace !== rowAttributes.place) {
                if (currentCompany !== undefined || currentCompany !== undefined) {
                    // save previous list
                    addModelData(currentPlace, companyEventsList)
                    companyEventsList = []
                }
                currentCompany = rowAttributes.company
                currentPlace = rowAttributes.place
            }

            var dataObject = createDataObject(rowAttributes);
            dataObject["row"] = row
            companyEventsList.push(dataObject)
        }

        // save last list
        if (currentCompany !== undefined || currentCompany !== undefined) {
            addModelData(currentPlace, companyEventsList)
        }
    }

    function enterEventDetailsPage(row) {
        var rowAttributes = shipmentEventsProxyModel.getRowAttributes(row);
        pageManager.enter(Enums.Page.EventDetails, { "attributes": createDataObject(rowAttributes) })
    }

    Flickable {
        id: mainFlickable

        anchors {
            fill: parent
            leftMargin: s(Style.bigMargin)
            rightMargin: s(Style.bigMargin)
            topMargin: s(Style.middleMargin)
        }

        clip: true
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.OvershootBounds

        contentWidth: width
        contentHeight: mainRowLayout.implicitHeight + s(Style.bigMargin)

        property var imagesModel

        RowLayout {
            id: mainRowLayout
            width: parent.width
            layoutDirection: Qt.RightToLeft
            spacing: s(10)

            property int childCount: 2
            property int availableWidth: width - mainRowLayout.spacing * childCount

            // From right to left:
            /// 1. Right-hand side delagtes
            ColumnLayout {
                Layout.fillHeight: false
                Layout.preferredWidth: 0.5 * parent.availableWidth
                Layout.leftMargin: s(Style.tinyMargin)
                spacing: s(Style.bigMargin) * 1.2

                Repeater {
                    id: shipmentRep
                    model: shipmentModel
                    delegate: Components.ShipmentDetailsDelegate {
                        spacing: s(Style.bigMargin) * 1.2

                        onDelegateClicked: enterEventDetailsPage(row)
                    }
                }
            }

            /// 2. Middle tree lines.
            Canvas {
                Layout.fillHeight: true
                Layout.preferredWidth: 0.2 * parent.availableWidth

                property color backColor: Style.pageBaseBackgroundColor
                property color linesColor: Style.currentCommodityColor
                property real lineWidth: sr(8)
                property real ringRadius: sr(14)
                property real ringThick: sr(3)

                function drawTreeLevel(ctx2d, referenceX, yMidPos, lineLen) {
                    var containerLen = yMidPos.length
                    if (containerLen === 0)
                        return {"minY" : -1, "maxY" : -1}

                    var min = 1000000
                    var max = 0

                    //var yMidLocal = mapFromItem(mainRowLayout, 0, yMidPos[0]).y
                    var yMidLocal =  yMidPos[0]
                    for (var j = 0; j < containerLen; ++j) {
                        min = Math.min(min, yMidLocal)
                        max = Math.max(max, yMidLocal)

                        ctx2d.beginPath()
                        ctx2d.moveTo(referenceX, yMidLocal)
                        ctx2d.lineTo((referenceX - lineLen), yMidLocal)

                        // Draw line between points except last point as there is no next one.
                        if (j !== (containerLen - 1)) {
                            //yMidLocal =  mapFromItem(mainRowLayout, 0, yMidPos[j+1]).y
                            yMidLocal =  yMidPos[j+1]
                            ctx2d.lineTo((referenceX - lineLen), yMidLocal + lineWidth * 0.5)
                        }

                        ctx2d.stroke()
                    }

                    return {"minY" : min, "maxY" : max}
                }

                function drawRing(ctx2d, referenceX, yMidPos) {
                    var cX = -1
                    var cY = -1

                    for (var i = 0; i < yMidPos.length; ++i) {
                        cX = referenceX
                        cY = yMidPos[i]

                        ctx2d.beginPath();
                        ctx2d.fillStyle = linesColor
                        ctx2d.moveTo(cX, cY);
                        ctx2d.arc(cX, cY, ringRadius, 0, Math.PI * 2, false);
                        ctx2d.fill();

                        ctx2d.beginPath();
                        ctx2d.fillStyle = backColor
                        ctx2d.moveTo(cX, cY);
                        ctx2d.arc(cX, cY, ringRadius - ringThick, 0, Math.PI * 2, false);
                        ctx2d.fill();
                    }
                }

                onPaint: {
                    var ctx = getContext("2d")

                    ctx.fillStyle = backColor
                    ctx.fillRect(0, 0, width, height)

                    ctx.lineWidth = lineWidth
                    ctx.strokeStyle = linesColor

                    var lineLen = width * 0.5 - ringRadius * 0.5 // subtract ring radius. Otherwise circle will be cut
                    var referenceX = width
                    var midPoints = []
                    var placeTypes = []

                    /// I. Draw first level of tree (right-hand side).
                    for (var i = 0; i < shipmentRep.count; ++i) {
                        var child = shipmentRep.itemAt(i)
                        var yMidPos = child.midYPos
                        for (var j = 0; j < yMidPos.lenght; ++j)
                            yMidPos[j] = mapFromItem(mainRowLayout, 0, yMidPos[j]).y

                        var minMax = drawTreeLevel(ctx, referenceX, yMidPos, lineLen)

                        placeTypes.push(child.placeType)
                        midPoints.push((minMax.maxY - minMax.minY) * 0.5 + minMax.minY)
                    }
                    referenceX = referenceX - lineLen

                    /// II. Draw second level of tree (left-hand side).
                    drawTreeLevel(ctx, referenceX, midPoints, lineLen)
                    referenceX = referenceX - lineLen

                    /// III. Draw circles on the lines at image positions.
                    drawRing(ctx, referenceX, midPoints)

                    var imgModel = []
                    for (i = 0; i < midPoints.length; ++i)
                        imgModel.push({"placeType" : placeTypes[i], "imageMidYPos" : midPoints[i]})

                    mainFlickable.imagesModel = imgModel
                }
            }

            /// 3. Left-hand size images
            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: 0.3 * parent.availableWidth

                Repeater {
                    model: mainFlickable.imagesModel
                    delegate: Items.SvgImage {
                        readonly property real imageMultiplier: {
                            // Pixel perfect resizing images.
                            // Some images has bigger width / height ratio than
                            // others. Every image need to be similar size.
                            switch(modelData.placeType) {
                            case Enums.PlaceType.Export:
                            case Enums.PlaceType.Village:
                                return 0.9
                            case Enums.PlaceType.Bagging:
                            case Enums.PlaceType.Nursery:
                                return 1.25
                            case Enums.PlaceType.Truck:
                                return 0.75
                            default:
                                return 1.0
                            }
                        }

                        width: parent.width
                        height: s(100) * imageMultiplier
                        y: modelData.imageMidYPos - height * 0.5
                        anchors.horizontalCenter: parent.horizontalCenter

                        fillMode: Image.PreserveAspectFit
                        source: Helpers.placeTypeToUrl(modelData.placeType)
                    }
                }
            }
        }
    }
}
