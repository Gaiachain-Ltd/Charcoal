import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

ColumnLayout {
    id: top

    Layout.fillHeight: false

    property string shipmentType: type
    property var midYPos: []

    onYChanged: {        
        for (var i = 0; i < rep.count; ++i)
            rep.itemAt(i).updateMidYPos()
    }

    Repeater {
        id: rep
        model: attributes

        delegate: RowLayout {
            id: delegateId
            Layout.fillHeight: false

            function updateMidYPos() {
                updateMidYPosInternal(index, height)
            }

            function updateMidYPosInternal(idx, h) {
                midYPos[idx] = mapToItem(mainRowLayout, 0, h / 2).y                
            }

            onYChanged: updateMidYPosInternal(index, height)
            Component.onCompleted: updateMidYPosInternal(index, height)

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0
                BasicText {
                    Layout.fillWidth: true
                    Layout.preferredHeight: s(100)
                    horizontalAlignment: Text.AlignLeft
                    text: title
                }
                BasicText {
                    Layout.fillWidth: true
                    Layout.preferredHeight: s(100)
                    horizontalAlignment: Text.AlignLeft
                    text: contentText
                }
            }

            ImageButton {
                Layout.preferredWidth: s(60)
                Layout.fillHeight: true

                fillMode: Image.PreserveAspectFit
                source: Style.rightArrowImgUrl
            }
        }
    }
}
