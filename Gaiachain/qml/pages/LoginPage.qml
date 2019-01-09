import QtQuick 2.11

import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top
    page: Enums.Page.Login

    headerVisible: false

    function enterViewTypePage() {
        pageManager.enter(Enums.Page.ViewType)
    }

    Connections
    {
        target: sessionManager
        onLoginFinished: enterViewTypePage()
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

                readonly property int calcWidth: parent.width * 0.7
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

                    source: Style.emailImgUrl
                    showImage: true

                    text: "producer@gaiachain.io" //TO_DO remove it in production
                    placeholderText: Strings.emailAddress

                    nextInput: passwordInput
                }

                Items.GenericInput
                {
                    id: passwordInput
                    Layout.preferredWidth: parent.width * 0.9
                    Layout.preferredHeight: s(Style.inputHeight)
                    Layout.alignment: Qt.AlignHCenter

                    source: Style.keyImgUrl
                    showImage: true
                    isPassword: true

                    text: "test1234" //TO_DO remove it in production
                    placeholderText: Strings.password

                    onMoveToNextInput: loginButton.clicked()
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

                        fillMode: Image.PreserveAspectFit
                        textFont.pixelSize: s(Style.pixelSize)

                        enabled: loginInput.text.length > 0 && passwordInput.text.length > 0 && utility.validateEmail(loginInput.text)
                        opacity: enabled ? 1 : 0.5

                        width: s(Style.buttonHeight) * 2.5

                        onClicked: {
                            sessionManager.login(loginInput.text, passwordInput.text)
                        }
                    }

                    Items.ImageButton
                    {
                        id: skipLoginButton
                        backgroundColor: Style.buttonBlackGreyColor
                        textColor: Style.textSecondaryColor
                        text: Strings.skipLogin
                        source: Style.skipArrowImgUrl

                        fillMode: Image.PreserveAspectFit

                        imageSize: s(Style.imageSize)
                        textFont.pixelSize: s(Style.pixelSize)

                        width: s(Style.buttonHeight) * 3.25

                        onClicked: {
                            userManager.userType = Enums.UserType.NotLoggedUser
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
        visible: overlay.loginRequest
    }
}
