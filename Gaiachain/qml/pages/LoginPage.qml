import QtQuick 2.11

import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

BasePage {
    id: top
    page: Enums.Page.Login

    headerVisible: false
    footerVisible: false

    function enterNextPage() {
        pageManager.enter(Enums.Page.ResourceChosing)
    }

    function closeEventHandler() {
        return true // android back button will close app
    }

    Connections
    {
        target: sessionManager
        onLoginFinished: enterNextPage()
        onDisplayLoginError: {
            // If someone send displayLoginError when not in login screen, ignore it and print warning
            if (!pageManager.isOnTop(page)) {
                console.warn("displayLoginError send when LoginPage is not on the top! Returning.")
                return
            }

            console.log("ON_LOGIN_ERROR",code)
            pageManager.enterPopup(Enums.Popup.Information, {
                                       "text" : Helpers.isNetworkError(code) ? Strings.noInternet : Strings.loginErrorInfo,
                                   })
        }
    }

    Connections {
        target: pageManager
        // When using popup always add checking if I'm on top
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            switch(action) {
            case Enums.PopupAction.Accept:
                console.log("Accept action to implement!")
                break
            default:
                break
            }
        }
    }

    ColumnLayout
    {
        id: layout
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
        }

        spacing: s(Style.smallMargin) * 1.5

        Item    // Item is to keep keyboard working properly
        {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.35

            // FIX_ME biding loops
            Items.SvgImage
            {
                anchors.centerIn: parent

                source: Style.logoImgUrl

                readonly property real calcWidth: parent.width * Style.logoSize
                width: calcWidth
            }
        }

        Items.LayoutSpacer {}

        Items.GenericInput
        {
            id: loginInput
            Layout.fillWidth: true

            nextInput: passwordInput

            additionalInputMethodHints: Qt.ImhNoAutoUppercase
            placeholderText: Style.loginByCombobox ? "" : Strings.emailAddress
            iconSource: Style.emailImgUrl

            readOnly: Style.loginByCombobox


            ComboBox    // this is for testing only !!!
            {
                id: controlCombo
                property string currentLoginStr: "producer@gaiachain.io"

                model: ["producer@gaiachain.io", "exporter@gaiachain.io"]
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
            Layout.maximumHeight: loginButton.height * 2
        }

        Items.GenericButton
        {
            id: loginButton
            Layout.fillWidth: true

            text: Strings.login

            enabled: Style.loginByCombobox || (loginInput.text.length > 0 && passwordInput.text.length > 0 && utility.validateEmail(loginInput.text))

            onClicked: {
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
                userManager.skipLogin()
                sessionManager.getEntity()
                top.enterNextPage()
            }
        }

        Items.LayoutSpacer {
            Layout.maximumHeight: loginButton.height * 0.4
        }
    }
}
