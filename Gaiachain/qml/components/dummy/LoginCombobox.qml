import QtQuick 2.11
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

// this is for testing only !!!
ComboBox
{
    id: top
    readonly property bool useEasyLogin: mainController.isEasyLogin

    property var loginInput
    property var passwordInput

    function updateInputsProperties() {
        loginInput.text = useEasyLogin ?  Qt.binding(function() { return currentText }) : ""
        passwordInput.text = useEasyLogin ?  useEasyLogin : ""
    }

    Component.onCompleted: updateInputsProperties()
    onUseEasyLoginChanged: updateInputsProperties()

    model: useEasyLogin ? mainController.easyLoginList : {}
    visible: useEasyLogin

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
            color: GStyle.textPrimaryColor
            font: loginInput.font

            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        background: Item {}
    }
}
