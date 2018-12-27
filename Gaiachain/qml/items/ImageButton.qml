import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.11

Item {
    id: top

    property string buttonSource: undefined
    property bool enableShadow: true
    property color dropShadowColor: "#80000000"

    property size buttonSize: Qt.size(s(140), s(140))

    signal buttonClicked()

    default property alias content: bottomContent.children

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: rButton.height

            DropShadow {
                anchors.fill: rButton
                source: rButton
                horizontalOffset: -s(10)
                verticalOffset: s(10)
                radius: 8.0
                samples: 17
                color: "#80000000"
            }

            RoundButton {
                id: rButton

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                width: top.buttonSize.width
                height: top.buttonSize.height

                background: Image {
                    sourceSize.width: width
                    sourceSize.height: height

                    source: top.buttonSource
                    fillMode: Image.PreserveAspectFit
                }

                radius: s(10)
                display: AbstractButton.IconOnly

                onClicked: top.buttonClicked()
            }
        }

        Item {
            id: bottomContent
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
