import QtQuick 2.11

import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top
    page: Enums.Page.Login

    headerVisible: false
    footerVisible: false

    function closeEventHandler() {
        return true // android back button will close app
    }

    ColumnLayout
    {
        id: layout
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
        }

        spacing: s(Style.middleSmallMargin)

        Items.LayoutSpacer  // It is to keep keyboard working properly
        {
            Layout.minimumHeight: logoImage.paintedHeight
            preferredHeight: parent.height * 0.42 - 2 * layout.spacing   // remove spacing for LayoutSpacer
            Layout.maximumHeight: preferredHeight

            // FIXME biding loops
            Items.SvgImage
            {
                id: logoImage
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                height: s(Style.logoHeight)

                source: Style.logoImgUrl
            }
        }

        Items.LayoutSpacer {}

        Items.GenericInput
        {
            id: loginInput
            Layout.fillWidth: true

            property bool validInput: true

            nextInput: passwordInput

            additionalInputMethodHints: Qt.ImhNoAutoUppercase
            placeholderText: Style.loginByCombobox ? "" : Strings.emailAddress
            iconSource: Style.emailImgUrl

            readOnly: Style.loginByCombobox

            borderColor: validInput ? Style.inputBorderColor : Style.errorColor

            onTextChanged: {
                validInput = !length || utility.validateEmail(text)
            }

            ComboBox    // this is for testing only !!!
            {
                id: controlCombo
                property string currentLoginStr: "producer@gaiachain.io"

                model: ["producer@gaiachain.io", "bagging@gaiachain.io",
                        "storekeeper@gaiachain.io", "exporter@gaiachain.io",
                        "wrong@gaiachain.io"]
                visible: Style.loginByCombobox

                anchors.centerIn: parent
                height: parent.height
                width: parent.width

                background: Rectangle { color: "transparent" }

                contentItem: Text {
                    leftPadding: loginInput.height

                    text: controlCombo.displayText
                    color: Style.textPrimaryColor
                    font {
                        pixelSize: s(Style.pixelSize)
                        family: Style.primaryFontFamily
                    }
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }

                delegate: ItemDelegate {
                    leftPadding: loginInput.height
                    width: parent.width
                    contentItem: Text {
                        text: modelData
                        color: Style.textPrimaryColor
                        font {
                            pixelSize: s(Style.pixelSize)
                            family: Style.primaryFontFamily
                        }
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }

                    background: Rectangle {
                        color: "transparent"
                    }
                }

                onActivated: {
                    passwordInput.text = "test1234"
                    switch(index) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        controlCombo.currentLoginStr = model[index]
                        break
                    default:
                        controlCombo.currentLoginStr = ""
                        passwordInput.text = ""
                        break
                    }
                }
            }
        }

        Items.GenericInput
        {
            id: passwordInput
            Layout.fillWidth: true

            placeholderText: Strings.password
            iconSource: Style.passwordImgUrl
            enabled: !Style.loginByCombobox
            isPassword: true

            text: Style.loginByCombobox ?  "test1234" : ""

            onMoveToNextInput: {
                if (loginButton.enabled) {
                    loginButton.clicked()
                }
            }
        }

        Items.LayoutSpacer {
            preferredHeight: 5 * s(Style.middleMargin)
            Layout.maximumHeight: preferredHeight
        }

        Items.GenericButton
        {
            id: loginButton
            Layout.fillWidth: true

            text: Strings.login

            enabled: Style.loginByCombobox || (loginInput.text.length && passwordInput.text.length && loginInput.validInput)

            onClicked: {
                pageManager.enter(Enums.Page.LoginLoading)

                var userName = Style.loginByCombobox ? controlCombo.currentLoginStr : loginInput.text
                sessionManager.login(userName, passwordInput.text)
            }
        }

        Items.GenericButton
        {
            id: skipLoginButton
            Layout.fillWidth: true

            palette {
                button: Style.buttonSecondaryColor
                buttonText: Style.textPrimaryColor
            }
            borderColor: Style.inputBorderColor

            text: Strings.skipLogin

            onClicked: {
                pageManager.enter(Enums.Page.LoginLoading, { "skipLogin": true })

                userManager.skipLogin()
                sessionManager.getEntity()
            }
        }

        Items.LayoutSpacer {
            preferredHeight: s(Style.bigMargin)
            Layout.maximumHeight: preferredHeight
        }
    }
}
