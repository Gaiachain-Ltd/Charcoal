import QtQuick 2.11

import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items
import "../components/dummy" as DummyComponents

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
            placeholderText: Strings.emailAddress
            iconSource: Style.emailImgUrl

            borderColor: validInput ? Style.inputBorderColor : Style.errorColor

            onTextChanged: {
                validInput = !length || Utility.validateEmail(text)
            }

            DummyComponents.DummyLoginCombobox {
                loginInput: loginInput
                passwordInput: passwordInput
            }
        }

        Items.GenericInput
        {
            id: passwordInput
            Layout.fillWidth: true

            placeholderText: Strings.password
            iconSource: Style.passwordImgUrl
            isPassword: true

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

            enabled: loginInput.text.length && passwordInput.text.length && loginInput.validInput

            onClicked: {
                pageManager.enter(Enums.Page.LoginLoading)
                sessionManager.login(loginInput.text, passwordInput.text)
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
                sessionManager.ping()
                pageManager.enter(Enums.Page.LoginLoading, { "skipLogin": true })
            }
        }

        Items.LayoutSpacer {
            preferredHeight: s(Style.bigMargin)
            Layout.maximumHeight: preferredHeight
        }
    }
}
