import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

ColumnLayout {
    id: top
    Layout.fillHeight: false
//    width: parent.width

    property string shipmentType: type
    spacing: s(5)
    property int bottomItemHeight: s(70)

    property var midYPos: []// getArrayOfSize(attributes.count)

//    property var delegatesHeights: getArrayOfSize(attributes.count)

    onYChanged: {
        console.log("Y changed parent !!!", title, y)
        for (var i = 0; i < rep.count; ++i) {
            rep.itemAt(i).updateMidYPosTEST();
        }
    }

    Repeater {
        id: rep
        model: attributes

        delegate: RowLayout {
            id: delegateId
            Layout.fillHeight: false
//            Layout.fillWidth: true

            function updateMidYPosTEST() {
                updateMidYPos(index, y, height)
            }
            function updateMidYPos(idx, y, h) {
//                console.log("hhh", h)
//                delegatesHeights[idx] = h

                console.log("Y changed !!!", title, y)
                midYPos[idx] = mapToItem(mainRowLayout, 0, h / 2).y
                console.log("Pos", title, idx, y, h/2, midYPos[idx])
            }

            onYChanged: updateMidYPos(index, y, height)
            Component.onCompleted: updateMidYPos(index, y, height)

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0
                BasicText {
                    Layout.fillWidth: true
                    Layout.preferredHeight: s(100)
                    text: title

                    Rectangle {
                        anchors.fill: parent
                        color: "red"
                        opacity: 0.6
                    }
                }
                BasicText {
                    Layout.fillWidth: true
                    Layout.preferredHeight: s(100)
                    text: contentText

                    Rectangle {
                        anchors.fill: parent
                        color: "green"
                        opacity: 0.6
                    }
                }
            }

            ImageButton {
                Layout.preferredWidth: s(70)
                Layout.fillHeight: true

                fillMode: Image.PreserveAspectFit
                source: Style.rightArrowImgUrl
            }
        }
    }

    Item {
        width: parent.width
        height: bottomItemHeight
    }
}
