import QtQuick 2.11

import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import Qt.labs.settings 1.1

import "../../pages"
import "../../items" as Items
import "../../components/dummy" as DummyComponents

GPage {
    id: top
    page: Enums.Page.Login

    headerVisible: false
    footerVisible: false

    function closeEventHandler() {
        return true // android back button will close app
    }

    Settings {
        id: loginSettings

        category: "Login"

        property string login

        Component.onCompleted: {
            if (!mainController.isEasyLogin) {
                loginInput.text = login
            }
        }
    }

    Connections {
        target: sessionManager

        onLoginFinished: {
            loginSettings.login = login
        }
    }

    ColumnLayout
    {
        id: layout
        anchors {
            fill: parent
            margins: s(GStyle.bigMargin)
        }

        spacing: s(GStyle.middleSmallMargin)

        Items.LayoutSpacer  // It is to keep keyboard working properly
        {
            Layout.minimumHeight: logoImage.paintedHeight
            preferredHeight: parent.height * 0.42 - 2 * layout.spacing   // remove spacing for LayoutSpacer
            Layout.maximumHeight: preferredHeight

            Items.SvgImage
            {
                id: logoImage
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                height: s(GStyle.logoHeight)

                source: GStyle.logoImgUrl

                DummyComponents.ServerStateChanger {}
            }
        }

        Items.LayoutSpacer {}

        Items.GInput
        {
            id: loginInput
            Layout.fillWidth: true

            property bool validInput: true

            nextInput: passwordInput

            additionalInputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhNoAutoUppercase
            placeholderText: Strings.emailAddress
            iconSource: GStyle.emailImgUrl

            borderColor: validInput ? GStyle.inputBorderColor : GStyle.errorColor

            onTextChanged: {
                validInput = !length || Utility.validateEmail(text)
            }

            DummyComponents.LoginCombobox {
                loginInput: loginInput
                passwordInput: passwordInput
            }
        }

        Items.GInput
        {
            id: passwordInput
            Layout.fillWidth: true

            placeholderText: Strings.password
            iconSource: GStyle.passwordImgUrl
            isPassword: true

            onAccepted: {
                if (loginButton.enabled) {
                    loginButton.clicked()
                }
            }
        }

        Items.LayoutSpacer {
            preferredHeight: 5 * s(GStyle.middleMargin)
            Layout.maximumHeight: preferredHeight
        }

        Items.GButton
        {
            id: loginButton
            Layout.fillWidth: true

            text: Strings.login

            enabled: loginInput.text.length && passwordInput.text.length && loginInput.validInput

            onClicked: {
                pageManager.enter(Enums.Page.LoginLoading, { "login": loginInput.text, "password": passwordInput.text })
                sessionManager.login(loginInput.text, passwordInput.text)
            }
        }

        Items.GButton
        {
            id: skipLoginButton
            Layout.fillWidth: true

            palette {
                button: GStyle.buttonSecondaryColor
                buttonText: GStyle.textPrimaryColor
            }
            borderColor: GStyle.inputBorderColor

            text: Strings.skipLogin

            onClicked: {
                pageManager.enter(Enums.Page.LoginLoading, { "skipLogin": true })
                sessionManager.getAdditionalData()
            }
        }

        Items.LayoutSpacer {
            preferredHeight: s(GStyle.bigMargin)
            Layout.maximumHeight: preferredHeight
        }
    }
}
