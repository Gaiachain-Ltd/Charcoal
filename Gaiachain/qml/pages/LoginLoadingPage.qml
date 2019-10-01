import QtQuick 2.11

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

Items.GenericPanel
{
    id: top
    page: Enums.Page.LoginLoading

    function closeEventHandler() {
        return false    // back button not working for this page
    }

    property bool skipLogin: false

    Connections
    {
        target: sessionManager
        onLoginFinished: d.onLoginReady()
        onLoginError: d.showLoginError(code)
    }

    states: [
        State {
            name: "zero"
            PropertyChanges { target: logoLoading; source: Style.loadingLogoOneImgUrl }
            PropertyChanges { target: logoLoading; opacity: 0 }
        },
        State {
            name: "one"
            PropertyChanges { target: logoLoading; source: Style.loadingLogoOneImgUrl }
        },
        State {
            name: "two"
            PropertyChanges { target: logoLoading; source: Style.loadingLogoTwoImgUrl }
        },
        State {
            name: "three"
            PropertyChanges { target: logoLoading; source: Style.loadingLogoThreeImgUrl }
        }
    ]

    QtObject {
        id: d

        // navigation
        property bool animationFinished: false
        property bool loginReady: top.skipLogin

        function onLoginReady() {
            loginReady = true
            sessionManager.getEntity()
        }
        function onAnimationFinished() {
            animationFinished = true
        }

        function checkEnterMainPage() {
            if (!animationFinished || !loginReady) {
                return false
            }

            if (pageManager.isOnTop(top.page)) {
                pageManager.enterReplace(Enums.Page.MainMenu)
            } else {
                console.warn("Login finished when", top.page, "is not on the top! Returning.")
            }
            return true
        }
        function showLoginError(code) {
            console.log("Login Error:", code)

            if (pageManager.isOnTop(top.page)) {
                pageManager.back()
                pageManager.openPopup(Enums.Popup.Information,
                                      { "text": Helpers.isNetworkError(code) ? Strings.noInternetError : Strings.loginError })
            } else {
                console.warn("Login error send when", top.page, "is not on the top! Returning.")
            }
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
        interval: 500
        onTriggered: d.nextState()
    }

    Items.SvgImage {
        id: logoLoading
        anchors.centerIn: parent
    }

    Items.BasicText {
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: s(Style.hugeMargin)
        }

        font {
            pixelSize: s(Style.titlePixelSize)
            capitalization: Font.AllUppercase
            letterSpacing: s(Style.buttonLetterSpacing)
            weight: Font.DemiBold
        }
        color: Style.textDisabledColor

        text: Strings.loading
    }
}
