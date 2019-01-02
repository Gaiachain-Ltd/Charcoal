import QtQuick 2.11

import com.gaiachain.style 1.0

Item {
    id: top

    property alias viewModel: mainView.model
    property alias delegateHeight: mainView.delegateHeight
    property alias backgroundColor: background.color

    Rectangle {
        id: background
        anchors.fill: parent

        color: "transparent"
    }

    ListView {
        id: mainView

        anchors.fill: parent
        clip: true

        property real delegateHeight: s(50)

        delegate: Item {
            height: ListView.view.delegateHeight
            width: ListView.view.width

            ImageItem {
                anchors.fill: parent
                imageUrl: Style.backImgUrl
                text: "TEST"
                layoutDirection: Qt.RightToLeft
            }

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                height: sr(1)
                width: utility.proportionalWidth(390)
                color: "#FFCCCCCC"
            }
        }
    }
}
