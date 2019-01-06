import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    property var imgUrls: [Style.timberImgUrl, Style.logParkImgUrl, Style.sawmillImgUrl, Style.exportImgUrl]
    function typeToUrl(shipmentType) {
        switch(shipmentType) {
        case "forestery": return Style.timberImgUrl
        case "logpark": return Style.logParkImgUrl
        case "sawmill": return Style.sawmillImgUrl
        case "export": return Style.exportImgUrl
        default:
            console.log("Wrong type of shipment:", shipmentType)
        }

        return ""
    }

    ListModel {
        id: shipmentModel

        ListElement {
            type: "forestery"
            attributes: [
                ListElement {
                    title: "Apple"
                    contentText: "Logs 1"
                },
                ListElement {
                    title: "Orange"
                    contentText: "Logs 2"
                },
                ListElement {
                    title: "Banana"
                    contentText: "Logs 3"
                }
            ]
        }

        ListElement {
            type: "logpark"
            attributes: [
                ListElement {
                    title: "Log park"
                    contentText: "Batch 1"
                }
            ]
        }

        ListElement {
            type: "sawmill"
            attributes: [
                ListElement {
                    title: "Sawmill"
                    contentText: "Batch 1"
                }
            ]
        }

        ListElement {
            type: "export"
            attributes: [
                ListElement {
                    title: "Timber for export"
                    contentText: "Batch 1"
                },
                ListElement {
                    title: "Timber for export"
                    contentText: "Batch 2"
                }
            ]
        }
    }

    Flickable {
        id: mainFlickable

        anchors.fill: parent
        anchors.margins: s(10)

        clip: true
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.OvershootBounds

        contentWidth: width
        contentHeight: mainRowLayout.implicitHeight

        property var mids: [{}]

        RowLayout {
            id: mainRowLayout
            width: parent.width
            layoutDirection: Qt.RightToLeft

            // From right to left:
            // TO_DO 1.Views
            ColumnLayout {
                Layout.fillHeight: true
                Layout.preferredWidth: 0.5 * parent.width

                Repeater {
                    id: shipmentRep
                    model: shipmentModel
                    delegate: Items.ShipmentDetailsDelegate {}
                }
            }

            // TO_DO 2. Canvas
            Canvas {
                Layout.fillHeight: true
                Layout.preferredWidth: 0.25 * parent.width

                onPaint: {
                    var ctx = getContext("2d")

                    ctx.fillStyle = "white"
                    ctx.fillRect(0,0,width ,height)

                    ctx.lineWidth = 5;
                    ctx.strokeStyle = "green"

                    var lastMax = 0
                    var lineLen = width/3
                    var midPoints = [{}]
                    for (var i = 0; i < shipmentRep.count; ++i) {
                        var child = shipmentRep.itemAt(i)
                        var yMidPos = shipmentRep.itemAt(i).midYPos
                        console.log("XXX")
                        for (var j = 0; j < yMidPos.length; ++j) {
                            console.log("CUrrent Y", lastMax + yMidPos[j])
                            ctx.beginPath()
                            ctx.moveTo(width, lastMax + yMidPos[j])
                            ctx.lineTo((width - lineLen), lastMax + yMidPos[j])
                            ctx.lineTo((width - lineLen), lastMax + (child.maxYPos - child.minYPos)/2 + child.minYPos)
                            ctx.stroke()
                        }
                        midPoints.push({"type" : child.shipmentType, "midYPos" : lastMax + (child.maxYPos - child.minYPos)/2 + child.minYPos})
                        lastMax = lastMax + child.maxYPos
                    }

                    for (i = 0; i < midPoints.length; ++i) {
                        ctx.beginPath()
                        ctx.moveTo(width - 2 * lineLen, midPoints[i].midYPos)
                        ctx.lineTo((width - lineLen), midPoints[i].midYPos)

                        // TO_DO add circle drawing
                        if (i !== (midPoints.length - 1)) {
                            ctx.moveTo(width - 2 * lineLen, midPoints[i].midYPos)
                            ctx.lineTo(width - 2 * lineLen, midPoints[i+1].midYPos)
                        }
                        ctx.stroke()
                    }

                    var cX = -1
                    var cY = -1
                    for (i = 0; i < midPoints.length; ++i) {
                        cX = width - 2 * lineLen
                        cY = midPoints[i].midYPos

                        ctx.beginPath();
                        ctx.fillStyle = "green"
                        ctx.moveTo(cX, cY);
                        ctx.arc(cX, cY, 10, 0, Math.PI * 2, false);
                        ctx.fill();

                        ctx.beginPath();
                        ctx.fillStyle = "white"
                        ctx.moveTo(cX, cY);
                        ctx.arc(cX, cY, 7, 0, Math.PI * 2, false);
                        ctx.fill();
                    }

                    mainFlickable.mids = midPoints
                }
            }

            // TO_DO 3. Images
            Item {
                id:imagesComp
                Layout.fillHeight: true
                Layout.preferredWidth: 0.25 * parent.width

                Repeater {
                    model: mainFlickable.mids
                    delegate: Items.SvgImage {
                        width: parent.width
                        height: Math.min(s(70), width)
                        y: modelData.midYPos - height / 2

                        fillMode: Image.PreserveAspectFit
                        source: typeToUrl(modelData.type)
                    }
                }
            }
        }
    }
}
