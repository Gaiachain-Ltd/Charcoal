import QtQuick 2.11
import QtQuick.Layouts 1.11

// TO_DO consider using Toolbar
Item {

    signal barClicked()

    Rectangle {
        id: background
        anchors.fill: parent
        color: "black"
    }

    RowLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        Rectangle {
            Layout.preferredWidth: 50
            Layout.fillHeight: true
            color: "green"
        }
        Text {
            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            rightPadding: 10
            leftPadding: 10

            color: "white"
            text: qsTr("Please register")
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
