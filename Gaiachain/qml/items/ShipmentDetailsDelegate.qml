import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

ColumnLayout {
    id: top
    width: parent.width
    property string shipmentType: type

    function getArrayOfSize(s){
        var a = []
        for (var i=0; i < s; ++i) {
            a.push(i *100 + 50)
            if ((i *100 + 50) < minYPos) {
                minYPos = (i *100 + 50)
            }

            if ((i *100 + 50) > maxYPos) {
                maxYPos = (i *100 + 50)
            }
        }

        return a
    }

    property var midYPos: getArrayOfSize(attributes.count)
    property real minYPos: 100000
    property real maxYPos: 0
    function updateMidYPos(idx, y, h) {
        var point = mapFromGlobal(0, 0);
        midYPos[idx] = point.y + y + h/2

    }

    Repeater {
        id: rep
        model: attributes

        delegate: RowLayout {
            Layout.fillWidth: true

            //onYChanged: updateMidYPos(index, y, height)
            //Component.onCompleted:  updateMidYPos(index, y, height)

            ColumnLayout {
                Layout.fillWidth: true
                BasicText {
                    Layout.fillWidth: true
                    text: title
                }
                BasicText {
                    Layout.fillWidth: true
                    text: contentText
                }
            }

            ImageButton {
                Layout.preferredWidth: s(50)
                Layout.fillHeight: true

                fillMode: Image.PreserveAspectFit
                source: Style.rightArrowImgUrl
            }
        }
    }

    Item {
        width: parent.width
        height: s(50)
    }
}
