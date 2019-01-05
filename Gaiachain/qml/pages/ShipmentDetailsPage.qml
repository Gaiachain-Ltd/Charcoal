import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

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

        RowLayout {
            id: mainRowLayout
            width: parent.width
            layoutDirection: Qt.RightToLeft

            function arrayContains(array, idx) {
                for (var i = 0; i < array.count; ++i) {
                    if (array[i].index === idx)
                        return i
                }

                return -1
            }

            // From right to left:
            // TO_DO 1.Views
            ColumnLayout {
                Layout.fillHeight: true
                Layout.preferredWidth: utility.proportionalWidth(0.5 * parent.width, parent.width)

                Repeater {
                    id: shipmentRep
                    model: shipmentModel
                    delegate: Items.ShipmentDetailsDelegate {}
                }
            }

            property var mids: []
            // TO_DO 2. Canvas
            Canvas {
                Layout.fillHeight: true
                Layout.preferredWidth: utility.proportionalWidth(0.25 * parent.width, parent.width)

                onPaint: {
                    var ctx = getContext("2d")

                    ctx.fillStyle = "white"
                    ctx.fillRect(0,0,width ,height)

                    ctx.lineWidth = 5;
                    ctx.strokeStyle = "green"

                    var lastMax = 0
                    var lineLen = width/3
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
                        mainRowLayout.mids.push(lastMax + (child.maxYPos - child.minYPos)/2 + child.minYPos)
                        lastMax = lastMax + child.maxYPos
                    }

                    for (i = 0; i < mainRowLayout.mids.length; ++i) {
                        ctx.beginPath()
                        ctx.moveTo(width - 2 * lineLen, mainRowLayout.mids[i])
                        ctx.lineTo((width - lineLen), mainRowLayout.mids[i])

                        // TO_DO add circle drawing
                        if (i !== (mainRowLayout.mids.length - 1)) {
                            ctx.moveTo(width - 2 * lineLen, mainRowLayout.mids[i])
                            ctx.lineTo(width - 2 * lineLen, mainRowLayout.mids[i+1])
                        }
                        ctx.stroke()
                    }

                    var cX = -1
                    var cY = -1
                    for (i = 0; i < mainRowLayout.mids.length; ++i) {
                        cX = width - 2 * lineLen
                        cY = mainRowLayout.mids[i]

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
                }
            }

            // TO_DO 3. Images
            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: utility.proportionalWidth(0.25 * parent.width, parent.width)

                property var imgUrls: [Style.timberImgUrl, Style.logParkImgUrl, Style.sawmillImgUrl, Style.exportImgUrl]
                ListView {
                    interactive: false
                    model: mainRowLayout.mids
                    delegate: Items.SvgImage {
                        y: modelData
                        width: parent.width
                        height: s(50)

                        fillMode: Image.PreserveAspectFit
                        source: imgUrls[index]
                    }
                }


                //                Items.SvgImage {
                //                    y: mainRowLayout.mids[1]
                //                    width: parent.width
                //                    height: s(50)

                //                    fillMode: Image.PreserveAspectFit
                //                    source: Style.logParkImgUrl
                //                }

                //                Items.SvgImage {
                //                    y: mainRowLayout.mids[2]
                //                    width: parent.width
                //                    height: s(50)

                //                    fillMode: Image.PreserveAspectFit
                //                    source: Style.sawmillImgUrl
                //                }

                //                Items.SvgImage {
                //                    y: mainRowLayout.mids[3]
                //                    width: parent.width
                //                    height: s(50)

                //                    fillMode: Image.PreserveAspectFit
                //                    source: Style.exportImgUrl
                //                }

            }
        }
    }
}
