import QtQuick 2.11
import QtQuick.Layouts 1.11

Item {
    id: top

    property string imageUrl: undefined
    property string imageText: ""

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Image {
            id: innerImage
            Layout.fillHeight: true

            source: top.imageUrl
            sourceSize.width: s(150)
            sourceSize.height: s(150)
            fillMode: Image.PreserveAspectFit
        }

        Text {
            Layout.fillHeight: true

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: s(50)
            text: top.imageText
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

}
