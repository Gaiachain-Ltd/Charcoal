import QtQuick 2.11

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

GPage
{
    id: top
    page: Enums.Page.LoginLoading

    headerVisible: false
    footerVisible: false

    function closeEventHandler() {
        return false    // back button not working for this page
    }

    property string login
    property string password

    property bool skipLogin: false

    Connections
    {
        target: sessionManager
        enabled: Number(pageManager.topPage) === page

        function onLoginFinished(login, userDataObj) {
            d.onLoginReady()
        }

        function onLoginError(login, code) {
            d.handleLoginError(code)
        }
    }

    Connections
    {
        target: pageManager
        enabled: Number(pageManager.topPage) === page

        function onPopupAction(action, popupId) {
            if (popupId === "OFFLINE_LOGIN") {
                switch(action) {
                case Enums.PopupAction.Yes:
                    d.tryOfflineLogin()
                    break
                case Enums.PopupAction.No:  // rejecting offline mode
                default:
                    pageManager.back()
                }
            } else if (popupId === "LOGIN_ERROR") {
                switch(action) {
                case Enums.PopupAction.Ok:  // accepting error
                default:
                    pageManager.back()
                }
            }
        }
    }

    states: [
        State {
            name: "zero"
            PropertyChanges { target: logoLoading; opacity: 0 }
        },
        State {
            name: "one"
            PropertyChanges { target: logoLoading; source: GStyle.loadingLogoOneImgUrl }
        },
        State {
            name: "two"
            PropertyChanges { target: logoLoading; source: GStyle.loadingLogoTwoImgUrl }
        },
        State {
            name: "three"
            PropertyChanges { target: logoLoading; source: GStyle.loadingLogoThreeImgUrl }
        }
    ]

    QtObject {
        id: d

        // navigation
        property bool animationFinished: false
        property bool loginReady: top.skipLogin

        function onLoginReady() {
            loginReady = true
        }
        function onAnimationFinished() {
            animationFinished = true
        }

        function tryOfflineLogin() {
            if (userManager.offlineLogin(login, password)) {
                onLoginReady()
            } else {
                handleLoginError(RequestHelper.authenticationError())
            }
        }

        function checkEnterMainPage() {
            if (!animationFinished || !loginReady) {
                return false
            }

            pageManager.enterReplace(Enums.Page.MainMenu)
            return true
        }
        function handleLoginError(code) {
            if (RequestHelper.isOfflineError(code)) {
                if (userManager.offlineAvailable(login)) {
                    pageManager.openPopup(Enums.Popup.YesNoQuestion,
                                          { "text": Strings.offlineModeQuestion },
                                          "OFFLINE_LOGIN")
                    return
                }
            }

            pageManager.openPopup(Enums.Popup.Information,
                                  { "text": RequestHelper.isAuthenticationError(code) ? Strings.loginError : Strings.serverConnectionError },
                                  "LOGIN_ERROR")
        }

        // animation
        property int currentStateIdx: 0

        function updateState() {
            top.state = top.states[currentStateIdx].name
        }
        function nextState() {
            currentStateIdx = ++currentStateIdx
            if (currentStateIdx >= top.states.length) {
                onAnimationFinished()
                if (checkEnterMainPage()) {
                    timer.running = false;
                    return
                }

                currentStateIdx = 0 // restart
            }

            updateState()
        }
    }

    Component.onCompleted: d.updateState()

    Timer {
        id: timer
        running: true
        repeat: true
        interval: mainController.isEasyLogin? 250 : 500
        onTriggered: d.nextState()
    }

    Items.SvgImage {
        id: logoLoading
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
        height: s(GStyle.logoHeight)

        source: GStyle.loadingLogoOneImgUrl
    }

    Items.GText {
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 3 * s(GStyle.hugeMargin)
        }

        font {
            pixelSize: s(GStyle.subtitlePixelSize)
            capitalization: Font.AllUppercase
            letterSpacing: s(GStyle.buttonLetterSpacing)
            weight: Font.DemiBold
        }
        color: GStyle.loginLoadingTextColor

        text: Strings.loading
    }
}
