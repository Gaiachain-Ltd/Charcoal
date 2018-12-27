import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Item {
    id: top
    property bool footerVisible: true
    property bool headerVisible: true

    property color backgroundColor: "white"

    default property alias content: pageContent.children

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        Item {
            id: pageContent
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: background
                anchors.fill: parent
                color: top.backgroundColor
            }
        }

        ToolBar {
            id: toolbar
            Layout.fillWidth: true
            position: ToolBar.Footer

            RowLayout {
                anchors.fill: parent
                ToolButton {
                    text: qsTr("‹")
                }
                Label {
                    Layout.fillWidth: true

                    text: "Title"
                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
                ToolButton {
                    text: qsTr("⋮")
                }
            }
        }
    }
}
