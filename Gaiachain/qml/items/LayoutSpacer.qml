import QtQuick 2.11
import QtQuick.Layouts 1.11

Item {
    property bool horizontal: true
    property bool vertical: true

    property var preferredWidth: undefined
    property var preferredHeight: undefined

    Layout.fillWidth: horizontal
    Layout.fillHeight: vertical

    Layout.preferredWidth: preferredWidth !== undefined ? preferredWidth : implicitWidth
    Layout.preferredHeight: preferredHeight !== undefined ? preferredHeight : implicitHeight
}
