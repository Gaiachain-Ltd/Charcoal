import QtQuick 2.11

import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import Qt.labs.settings 1.1

import "../pages"
import "../items" as Items
import "../components/dummy" as DummyComponents

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

        function onLoginFinished(login, userDataObj) {
            loginSettings.login = login
        }
    }

    ColumnLayout
    {
        id: layout
        property int margin: s(GStyle.bigMargin)

        anchors {
            fill: parent
            bottomMargin: s(GStyle.hugeMargin)
        }

        spacing: s(GStyle.middleMargin)

        Items.LayoutSpacer  // It is to keep keyboard working properly
        {
            id: logoLayout
            Layout.minimumHeight: top.height * 0.35
            preferredHeight: parent.height * 0.37 - 2 * layout.spacing   // remove spacing for LayoutSpacer
            Layout.maximumHeight: preferredHeight

            Image {
                id: logoBackground
                source: GStyle.loginBackgroundUrl
                anchors.horizontalCenter: parent.horizontalCenter
                // height - |y| = 33% top.height
                y: -(height - top.height * 0.33)
                width: top.width * 1.3
                height: width * 0.7
            }

            Items.SvgImage
            {
                id: logoImage
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: s(75)
                }

                height: (top.height - Qt.inputMethod.keyboardRectangle.height) * 0.25
                source: GStyle.logoMalebiWhiteCharcoalUrl
                DummyComponents.ServerStateChanger {}
            }
        }

        Items.LayoutSpacer {
            Layout.maximumHeight: top.height * 0.33 - logoLayout.height
        }

        Items.GInput
        {
            id: loginInput
            Layout.fillWidth: true

            Layout.leftMargin: layout.margin
            Layout.rightMargin: layout.margin

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

            Layout.leftMargin: layout.margin
            Layout.rightMargin: layout.margin

            placeholderText: Strings.password
            iconSource: GStyle.passwordImgUrl
            isPassword: true

            onAccepted: {
                if (loginButton.enabled) {
                    loginButton.clicked()
                }
            }
        }

        Items.GButton
        {
            id: loginButton
            Layout.fillWidth: true

            Layout.leftMargin: layout.margin
            Layout.rightMargin: layout.margin

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
            Layout.leftMargin: layout.margin
            Layout.rightMargin: layout.margin

            palette {
                button: GStyle.buttonSecondaryColor
                buttonText: GStyle.textPrimaryColor
            }

            borderColor: GStyle.inputBorderColor

            text: Strings.skipLogin
            visible: false

            onClicked: {
                pageManager.enter(Enums.Page.LoginLoading, { "skipLogin": true })
                sessionManager.getAdditionalData()
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: layout.margin
            Layout.rightMargin: layout.margin
            Layout.alignment: Qt.AlignHCenter
            spacing: s(GStyle.bigMargin * 2)

            Items.SvgImage
            {
                id: logoMilo

                height: s(GStyle.logoHeight)
                source: GStyle.logoMiloUrl
                DummyComponents.ServerStateChanger {}
            }

            Items.SvgImage
            {
                id: logoGaia

                height: s(GStyle.logoHeight)
                source: GStyle.logoGaiaUrl
                DummyComponents.ServerStateChanger {}
            }
        }
    }
}
