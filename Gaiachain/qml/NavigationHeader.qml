import QtQuick 2.5
import QtQuick.Layouts 1.5

Item {
    signal headerClicked()

    Rectangle {
        id: background
        anchors.fill: parent
        color: "white"
    }

    RowLayout {
        anchors.fill: parent
        Rectangle {
            Layout.preferredWidth: 50
            Layout.fillHeight: true
            color: "red"
        }
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "green"
        }
        Rectangle {
            Layout.preferredWidth: 50
            Layout.fillHeight: true
            color: "blue"
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: headerClicked()
    }
}
