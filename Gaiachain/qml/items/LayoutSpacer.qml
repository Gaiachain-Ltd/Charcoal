import QtQuick 2.11
import QtQuick.Layouts 1.11

Item {
    property bool horizontal: true
    property bool vertical: true

    property var spacerWidth: undefined
    property var spacerHeight: undefined

    Layout.fillHeight: spacerHeight !== undefined ? false : vertical
    Layout.fillWidth: spacerWidth !== undefined ? false : horizontal

    Layout.preferredHeight: spacerHeight !== undefined ? spacerHeight : implicitHeight
    Layout.preferredWidth: spacerWidth !== undefined ? spacerWidth : implicitWidth
}
