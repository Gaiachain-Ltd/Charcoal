import QtQuick 2.11
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

// this is for testing only !!!
ComboBox
{
    id: top
    readonly property bool useDummy: Utility.useCombobox()

    property var loginInput
    property var passwordInput

    function updateInputsProperties() {
        loginInput.text = useDummy ?  Qt.binding(function() { return currentText }) : ""
        passwordInput.text = useDummy ?  "test1234" : ""
    }

    Component.onCompleted: updateInputsProperties()
    onUseDummyChanged: updateInputsProperties()

    model: typeof (fakeLogins) !== "undefined" ? fakeLogins : {}
    visible: useDummy

    anchors.centerIn: parent
    height: parent.height
    width: parent.width

    background: Item {}

    contentItem: Item {}    // login input is used instead

    delegate: ItemDelegate {
        leftPadding: loginInput.leftPadding
        width: parent.width

        contentItem: Text {
            text: modelData
            color: Style.textPrimaryColor
            font: loginInput.font

            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        background: Item {}
    }
}
