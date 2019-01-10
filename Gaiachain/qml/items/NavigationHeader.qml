import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

// TO_DO consider using Toolbar
Item {
    id: top

    signal headerClicked()

    property var currentResource: Enums.CommodityType.Timber //TO_DO set proper text after user set

    function getResourceName(res) {
        switch(res) {
        case Enums.CommodityType.Timber: return Strings.timber
        case Enums.CommodityType.Charcoal: return Strings.charcoal
        case Enums.CommodityType.Cocoa: return Strings.cocoa
        default:
            console.warn("Invalid resource type!")
        }

        return Strings.timber + "!" // Add "!" if invalid
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
            leftMargin: s(Style.bigMargin)
            rightMargin: s(Style.bigMargin)
            topMargin: s(Style.smallMargin)
            bottomMargin: s(Style.smallMargin)
        }

        spacing: 0

        RowLayout {
            id: topRow
            Layout.fillHeight: true
            Layout.fillWidth: true

            spacing: 0

            ImageButton {
                Layout.preferredHeight: s(Style.buttonHeight) * 0.9
                Layout.preferredWidth: s(Style.buttonHeight) * 0.9
                Layout.alignment: Qt.AlignVCenter

                property bool isOnHomePage: pageManager.isOnHomePage()
                source: {
                    if (!isOnHomePage) return Style.backImgUrl

                    return userManager.loggedIn ? Style.logoutImgUrl : Style.exitToLoginImgUrl
                }

                onClicked: {
                    if (userManager.loggedIn && isOnHomePage) {
                        pageManager.enterPopup(Enums.Page.InformationPopup, {
                                                   "text" : Strings.logoutQuestion,
                                                   "acceptButtonText": Strings.logout,
                                                   "rejectButtonText": Strings.cancel})
                    } else {
                        pageManager.back()
                    }
                }
            }

            LayoutSpacer {}

            ImageButton {
                Layout.preferredHeight: s(Style.buttonHeight)
                Layout.preferredWidth: s(Style.buttonHeight)

                Layout.alignment: Qt.AlignVCenter
                source: Style.homeImgUrl

                onClicked: pageManager.backTo(pageManager.homePage())
            }

            ListView {
                id: buttonList
                Layout.fillHeight: true
                Layout.preferredWidth: Math.max(sectionsModel.maxSectionsDepth() * (topRow.height + s(Style.headerArrowWidth)), topRow.width * 0.5 + delegateWidth * 0.8)

                orientation: ListView.Horizontal
                interactive: false
                spacing: 0

                model: sectionsModel

                readonly property int delegateWidth: s(Style.headerArrowWidth) + topRow.height

                delegate: Item {
                    id: delegateId
                    height: ListView.view.height
                    width: buttonList.delegateWidth + (delegateId.isLast ? imageButton.padding : 0)

                    property bool isLast: (ListView.view.count - 1) === index

                    RowLayout {
                        anchors.fill: parent

                        spacing: delegateId.isLast ? imageButton.padding : s(Style.smallMargin)

                        SvgImage {
                            Layout.preferredHeight: s(Style.headerArrowHeight)
                            Layout.preferredWidth: s(Style.headerArrowWidth)

                            source: Style.rightArrowLightImgUrl
                        }

                        ImageButton {
                            id: imageButton
                            // From delegate size and arrow size, image button will have squere shape.
                            // Do not set width: height, to force square shape as it leads to "holes" between arrow and button.
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            padding: s(Style.normalMargin)
                            inset: s(Style.tinyMargin) * 0.5

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
            font {
                pixelSize: s(Style.headerTitlePixelSize)
                capitalization: Font.AllUppercase
                family: Style.secondaryFontFamily
                letterSpacing: s(Style.pixelSize) * 0.1
            }
            text: getResourceName(currentResource)
        }
    }
}
