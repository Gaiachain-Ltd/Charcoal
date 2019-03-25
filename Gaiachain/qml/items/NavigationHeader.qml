import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

// TO_DO consider using Toolbar
Item {
    id: top

    signal headerClicked()

    property var currentResource: Enums.CommodityType.Timber //TO_DO set proper text after user set
    readonly property bool isOnHomePage: pageManager.isOnHomePage()

    function backHandler() {
        if (userManager.loggedIn && (isOnHomePage || pageManager.isOnTop(Enums.Page.ResourceChosing))) {
            pageManager.enterPopup(Enums.Popup.Information, {
                                       "text" : Strings.logoutQuestion,
                                       "acceptButtonText": Strings.logout,
                                       "rejectButtonText": Strings.cancel,
                                       "acceptButtonType": Enums.PopupAction.Logout
                                   })
        } else {
            pageManager.back()
        }
    }

    Connections {
        target: pageManager
        // When using popup always add checking if I'm on top
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            switch(action) {
            case Enums.PopupAction.Logout:
                userManager.logOut()
                pageManager.backTo(Enums.Page.Login) //TO_DO immediate generates bug
                break
            case Enums.PopupAction.Cancel:
            default:
            }
        }
    }

    function getTitleText(res) {
        var result = ""
        switch(res) {
        case Enums.CommodityType.Timber:
            result = Strings.timber
            break
        case Enums.CommodityType.Charcoal:
            result = Strings.charcoal
            break
        case Enums.CommodityType.Cocoa:
            result = Strings.cocoa
            break
        default:
            // Add "!" if invalid
            result = Strings.timber + "!"
            console.warn("Invalid resource type!")
        }

        var showUserType = userManager.userType !== Enums.UserType.NotLoggedUser
                           && (page === Enums.Page.QRScanner
                           || page === Enums.Page.EditableEventDetails)

        if (showUserType) {
            return result + " " + utility.userTypeToString(userManager.userType).replace("_", " ")
        } else {
            return result
        }
    }

    function getResourceUrl(res, green) {
        switch (res) {
        case Enums.CommodityType.Timber: return green ? Style.miniTimberGreenImgUrl : Style.miniTimberImgUrl
            //case Enums.CommodityType.Charcoal: return Style.charcoalImgUrl
            //case Enums.CommodityType.Cocoa: return Style.cocoaImgUrl
        default:
            console.warn("Invalid resource type. Return empty url!")
        }

        return ""
    }

    function sectionToUrl(section, green) {
        switch(section) {
        case Enums.PageSections.ViewTypeSection: return getResourceUrl(currentResource, green)
        case Enums.PageSections.CalendarSection: return green ? Style.miniCalendarGreenImgUrl : Style.miniCalendarImgUrl
        case Enums.PageSections.EditableEventDetailsSection: return green ? Style.editGreenImgUrl : Style.editImgUrl
        case Enums.PageSections.EventsListSection: return green ? Style.miniListGreenImgUrl : Style.miniListImgUrl
        case Enums.PageSections.EventDetailsSection: return green ? Style.detailsGreenImgUrl : Style.detailsImgUrl
        case Enums.PageSections.ShipmentDetailsSection: return green ? Style.timelineGreenImgUrl : Style.timelineImgUrl
        case Enums.PageSections.QRSection: return green ? Style.qrCodeGreenImgUrl : Style.qrCodeImgUrl

        case Enums.PageSections.DefaultSection: return ""

        default:
            console.warn("Invalid section!", section)
        }

        return ""
    }

    ColumnLayout {
        anchors {
            fill: parent
            topMargin: s(Style.smallMargin)
            bottomMargin: s(Style.smallMargin)
            leftMargin: topRow.calculatedSpacing
            rightMargin: topRow.calculatedSpacing
        }

        spacing: 0

        RowLayout {
            id: topRow
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true

            readonly property real buttonWidth: s(Style.buttonHeight)
            readonly property real calculatedSpacing: Math.round((top.width - (buttonWidth * Style.headerMaximumButtonCount)) / (Style.headerMaximumButtonCount + 1))

            spacing: 0

            Item {
                Layout.preferredWidth: topRow.buttonWidth
                Layout.preferredHeight: topRow.buttonWidth
                ImageButton {
                    height: topRow.buttonWidth * 0.9
                    width: topRow.buttonWidth * 0.9
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }

                    source: {
                        if (!isOnHomePage) return Style.backImgUrl

                        return userManager.loggedIn ? Style.logoutImgUrl : Style.exitToLoginImgUrl
                    }

                    onClicked: {
                        backHandler()
                    }
                }
            }

            Item
            {
                // empty item
                Layout.preferredWidth: topRow.calculatedSpacing
                Layout.preferredHeight: parent.height
            }

            ImageButton {
                Layout.preferredWidth: topRow.buttonWidth
                Layout.preferredHeight: topRow.buttonWidth

                source: Style.homeImgUrl

                onClicked: pageManager.backTo(pageManager.homePage())
            }

            ListView {
                id: buttonList
                Layout.fillHeight: true
                Layout.fillWidth: true

                orientation: ListView.Horizontal
                interactive: false
                spacing: 0

                model: sectionsModel

                readonly property int delegateWidth: topRow.calculatedSpacing + topRow.buttonWidth

                delegate: Item {
                    id: delegateId
                    height: ListView.view.height
                    width: buttonList.delegateWidth

                    property bool isLast: (ListView.view.count - 1) === index

                    RowLayout {
                        anchors.fill: parent

                        spacing: 0

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            SvgImage {
                                height: s(Style.headerArrowHeight)
                                width: s(Style.headerArrowWidth)
                                anchors {
                                    centerIn: parent
                                    // last arrow is moved a little to the left
                                    // because button is little bigger than other
                                    horizontalCenterOffset: delegateId.isLast ? -(parent.width * 0.5 - width) : 0
                                }

                                source: Style.rightArrowLightImgUrl
                            }
                        }


                        ImageButton {
                            id: imageButton
                            Layout.preferredWidth: topRow.buttonWidth
                            Layout.preferredHeight: topRow.buttonWidth

                            padding: (delegateId.isLast ? 1.1 : 1.5) * s(Style.smallMargin)

                            source: sectionToUrl(id, !delegateId.isLast)
                            backgroundColor: delegateId.isLast ? Style.buttonBackColor : "transparent"

                            onClicked: pageManager.backToSection(id)
                        }
                    }
                }
            }
        }

        BasicText {
            Layout.fillWidth: true

            color: Style.textGreenColor
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignHCenter
            font {
                pixelSize: s(Style.headerTitlePixelSize)
                capitalization: Font.AllUppercase
                family: Style.secondaryFontFamily
                letterSpacing: s(Style.pixelSize) * 0.1
            }
            text: getTitleText(currentResource)
        }
    }
}
