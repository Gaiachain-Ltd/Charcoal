import QtQuick 2.11
import QtQuick.Layouts 1.1

Item {
    id: top
    property alias titleText: title.text
    property alias contentText: content.text

    property int titleHeight: -1
    property int contentHeight: -1

    property alias spacing: mainColumn.spacing

    implicitHeight: mainColumn.implicitHeight
    implicitWidth: mainColumn.implicitWidth

    ColumnLayout {
        id: mainColumn

        anchors.centerIn: parent
        width: parent.width
        height: parent.height

        spacing: sr(3)

        GText {
            id: title

            Layout.fillWidth: true
            Layout.preferredHeight: top.titleHeight > 0 ? top.titleHeight: implicitHeight

            horizontalAlignment: Text.AlignLeft

            font.bold: true
        }

        GText {
            id: content

            Layout.fillWidth: true
            Layout.preferredHeight: top.contentHeight > 0 ? top.contentHeight : implicitHeight

            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.WordWrap
            maximumLineCount: 10
        }
    }
}
