import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.utility 1.0

ColumnLayout {
    id: top

    Layout.fillHeight: false

    property var placeType: place
    property var midYPos: []

    function enterEventDetailsPage() {
        pageManager.enter(Enums.Page.EventDetails)
    }

    onYChanged: {
        for (var i = 0; i < rep.count; ++i)
            rep.itemAt(i).updateMidYPos()
    }

    Repeater {
        id: rep
        model: events

        delegate: Item {
            id: delegateId

            Layout.fillWidth: true
            Layout.preferredHeight: mainLayout.implicitHeight

            function updateMidYPos() {
                midYPos[index] = mapToItem(mainRowLayout, 0, mainLayout.height / 2).y
            }

            onYChanged: updateMidYPos()
            Component.onCompleted: updateMidYPos()

            RowLayout {
                id: mainLayout

                anchors.left: parent.left
                anchors.right: parent.right

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0
                    BasicText {
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: s(Style.smallPixelSize)
                        text: Utility.placeTypeToString(place) + ":"
                    }
                    BasicText {
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: s(Style.smallPixelSize)
                        wrapMode: Text.WordWrap
                        font.italic: true
                        text: Utility.placeActionToStringAction(action) + " " + company
                    }
                    BasicText {
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: s(Style.smallPixelSize)
                        font.bold: true
                        text: Strings.batch + " " + shipmentId
                    }
                }

                ImageButton {
                    Layout.preferredWidth: s(Style.smallButtonHeight)
                    Layout.fillHeight: true

                    imageSize: s(Style.headerArrowHeight)

                    fillMode: Image.PreserveAspectFit
                    source: Style.rightArrowImgUrl
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: enterEventDetailsPage()
            }
        }
    }
}
