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
                },
                ListElement {
                    title: "Kiwi"
                    contentText: "Logs 4"
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
            /// 1. Right-hand side delagtes
            ColumnLayout {
                Layout.fillHeight: false
                Layout.preferredWidth: 0.5 * parent.width
                spacing: 0

                Repeater {
                    id: shipmentRep
                    model: shipmentModel
                    delegate: Items.ShipmentDetailsDelegate {}
                }
            }

            /// 2. Middle tree lines.
            Canvas {
                Layout.fillHeight: true
                Layout.preferredWidth: 0.3 * parent.width

                property color backColor: "yellow"
                property color linesColor: "green"
                property int lineWidth: s(9)
                property int ringRadius: s(12)
                property int ringThick: sr(5)

                onPaint: {
                    var ctx = getContext("2d")

                    //TO_DO add backColor
                    //TO_DO refactor delegates and finish them
                    //TO_DO Probably part of I and II could be shared

                    ctx.lineWidth = lineWidth
                    ctx.strokeStyle = linesColor

                    var lineLen = width / 2 - ringRadius / 2 // subtract circle radius. Otherwise circle will be cut
                    var referenceX = width
                    var midPoints = [{}]

                    /// I. Draw first level of tree (right-hand side).
                    for (var i = 0; i < shipmentRep.count; ++i) {
                        var child = shipmentRep.itemAt(i)
                        var yMidPos = child.midYPos

                        var min = 1000000
                        var max = 0
                        for (var j = 0; j < yMidPos.length; ++j) {
                            var yMidLocal = mapFromItem(mainRowLayout, 0, yMidPos[j]).y
                            min = Math.min(min, yMidLocal)
                            max = Math.max(max, yMidLocal)

                            ctx.beginPath()
                            ctx.moveTo(referenceX, yMidLocal)
                            ctx.lineTo((referenceX - lineLen), yMidLocal)

                            // Draw line between points
                            if (j !== (yMidPos.length - 1))
                                ctx.lineTo((referenceX - lineLen), mapFromItem(mainRowLayout, 0, yMidPos[j+1]).y)

                            ctx.stroke()
                        }

                        // Push middle of min middle and max middle of current type for example `forestry`.
                        midPoints.push({"type" : child.shipmentType, "midYPos" : (max - min)/2 + min})
                    }
                    referenceX = referenceX - lineLen

                    /// II. Draw second level of tree (left-hand side).
                    for (i = 0; i < midPoints.length; ++i) {
                        var yMid = midPoints[i].midYPos

                        ctx.beginPath()
                        ctx.moveTo(referenceX - lineLen, yMid)
                        ctx.lineTo(referenceX, yMid)

                        if (i !== (midPoints.length - 1)) {
                            ctx.moveTo(referenceX - lineLen, yMid)
                            ctx.lineTo(referenceX - lineLen, midPoints[i+1].midYPos)
                        }
                        ctx.stroke()
                    }
                    referenceX = referenceX - lineLen

                    var cX = -1
                    var cY = -1
                    /// III. Draw circles on the lines at image positions.
                    for (i = 0; i < midPoints.length; ++i) {
                        cX = referenceX
                        cY = midPoints[i].midYPos

                        ctx.beginPath();
                        ctx.fillStyle = linesColor
                        ctx.moveTo(cX, cY);
                        ctx.arc(cX, cY, ringRadius, 0, Math.PI * 2, false);
                        ctx.fill();

                        ctx.beginPath();
                        ctx.fillStyle = backColor
                        ctx.moveTo(cX, cY);
                        ctx.arc(cX, cY, ringRadius - ringThick, 0, Math.PI * 2, false);
                        ctx.fill();
                    }

                    mainFlickable.mids = midPoints // Set mid point to be used as images model
                }
            }

            /// 3. Left-hand size images
            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: 0.2 * parent.width

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
