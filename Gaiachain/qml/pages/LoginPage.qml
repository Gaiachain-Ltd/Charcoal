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

    function enterViewTypePage() {
        pageManager.enter(Enums.Page.ViewType)
    }

    function closeEventHandler() {
        return true // android back button will close app
    }

    Component.onCompleted: {
        // Temporarly set Timber fixed until chosing resoures will be available (ResourceChosingPage).
        commodityProxyModel.setCommodityType(Enums.CommodityType.Timber)
    }

    Connections
    {
        target: sessionManager
        onLoginFinished: enterViewTypePage()
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

    Column
    {
        anchors.fill: parent

        Item
        {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height * 0.5

            Items.SvgImage
            {
                anchors.centerIn: parent

                source: Style.logoImgUrl

                readonly property int calcWidth: parent.width * Style.logoSize
                width: calcWidth
                height: 0.35 * calcWidth
            }
        }

        Item
        {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height * 0.5

            ColumnLayout
            {
                id: itemColumn
                spacing: s(Style.smallMargin) * 1.5
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                Items.GenericInput
                {
                    id: loginInput
                    Layout.preferredWidth: parent.width * 0.9
                    Layout.preferredHeight: s(Style.inputHeight)
                    Layout.alignment: Qt.AlignHCenter
                    additionalInputMethodHints: Qt.ImhNoAutoUppercase

                    input.enabled: !Style.loginByCombobox

                    source: Style.emailImgUrl
                    showImage: true

                    placeholderText: Style.loginByCombobox ? "" : Strings.emailAddress

                    nextInput: passwordInput

                    property string currentLoginStr: "producer@gaiachain.io"

                    ComboBox //TO_DO_LATER remove it
                    {
                        id: controlCombo
                        model: ["producer@gaiachain.io", "logpark@gaiachain.io", "sawmill@gaiachain.io", "exporter@gaiachain.io"]
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
                                loginInput.currentLoginStr = model[index]
                                break
                            default:
                                loginInput.currentLoginStr = ""
                                passwordInput.text = ""
                                break
                            }
                        }
                    }
                }

                Items.GenericInput
                {
                    id: passwordInput
                    Layout.preferredWidth: parent.width * 0.9
                    Layout.preferredHeight: s(Style.inputHeight)
                    Layout.alignment: Qt.AlignHCenter

                    enabled: !Style.loginByCombobox

                    source: Style.keyImgUrl
                    showImage: true
                    isPassword: true

                    placeholderText: Strings.password
                    text: Style.loginByCombobox ?  "test1234" : ""

                    onMoveToNextInput: {
                        if (loginButton.enabled)
                            loginButton.clicked()
                    }
                }

                Row {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: itemColumn.spacing

                    spacing: s(Style.normalMargin)

                    Items.ImageButton
                    {
                        id: loginButton
                        backgroundColor: Style.buttonGreenColor
                        textColor: Style.textSecondaryColor
                        text: Strings.login
                        source: Style.loginImgUrl

                        imageSize: s(Style.imageSize)

                        enabled: Style.loginByCombobox || (loginInput.text.length > 0 && passwordInput.text.length > 0 && utility.validateEmail(loginInput.text))
                        opacity: enabled ? 1 : 0.5

                        width: s(Style.buttonHeight) * 2.5

                        onClicked: {
                            if (Style.loginByCombobox) {
                                sessionManager.login(loginInput.currentLoginStr, passwordInput.text)
                            } else {
                                sessionManager.login(loginInput.text, passwordInput.text)
                            }
                        }
                    }

                    Items.ImageButton
                    {
                        id: skipLoginButton
                        backgroundColor: Style.buttonBlackGreyColor
                        textColor: Style.textSecondaryColor
                        text: Strings.skipLogin
                        source: Style.skipArrowImgUrl

                        imageSize: s(Style.imageSize)

                        width: s(Style.buttonHeight) * 3.25

                        onClicked: {
                            userManager.userType = Enums.UserType.NotLoggedUser
                            sessionManager.getEntity()
                            top.enterViewTypePage()
                        }
                    }
                }
            }
        }
    }

    Items.WaitOverlay
    {
        anchors.fill: parent
        logoVisible: true

        visible: overlays.loginRequest
    }
}
