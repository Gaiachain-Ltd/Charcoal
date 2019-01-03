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

    function getResourceUrl(res) {
        switch (res) {
        case Enums.CommodityType.Timber: return Style.miniTimberGreenImgUrl
            //case Enums.CommodityType.Charcoal: return Style.charcoalImgUrl
            //case Enums.CommodityType.Cocoa: return Style.cocoaImgUrl
        default:
            console.warn("Invalid resource type. Return empty url!")
        }

        return ""
    }

    function sectionToUrl(section) {
        switch(section) {
        case Enums.PageSections.ViewTypeSection: return getResourceUrl(currentResource)
        case Enums.PageSections.CalendarSection: return Style.miniCalendarGreenImgUrl
        case Enums.PageSections.EventsListSection: return Style.miniListGreenImgUrl
        case Enums.PageSections.EventsDetailsSection: return Style.detailsGreenImgUrl
        case Enums.PageSections.ShipmentDetailsSection: return Style.timelineGreenImgUrl
        case Enums.PageSections.QRSection: return Style.qrCodeGreenImgUrl

        case Enums.PageSections.DefaultSection: return ""

        default:
            console.warn("Invalid section!")
        }

        return ""
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            LayoutSpacer { spacerWidth: s(20) }

            ImageButton {
                Layout.fillHeight: true
                Layout.preferredWidth: height

                padding: s(30)
                fillMode: Image.PreserveAspectFit
                source: Style.backImgUrl

                onClicked: pageManager.pop()
            }

            LayoutSpacer { spacerWidth: s(10) }

            ImageButton {
                Layout.fillHeight: true
                Layout.preferredWidth: height

                padding: s(30)
                fillMode: Image.PreserveAspectFit
                source: Style.homeImgUrl

                onClicked: pageManager.goToInitialPage()
            }


            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true

                orientation: ListView.Horizontal
                interactive: false

                model: sectionsModel

                onCountChanged: {
                    console.log("Current sections num", count)
                }

                delegate: Item {
                    height: ListView.view.height
                    width: delegateLayout.childrenRect.width

                    RowLayout {
                        id: delegateLayout
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        SvgImage {
                            Layout.fillHeight: true
                            Layout.preferredWidth: s(20)

                            fillMode: Image.PreserveAspectFit
                            source: Style.rightArrowImgUrl
                        }

                        ImageButton {
                            Layout.fillHeight: true
                            Layout.preferredWidth: height

                            fillMode: Image.PreserveAspectFit
                            source: sectionToUrl(id)
                            onSourceChanged: {
                                console.log("Current source", source)
                            }
                        }
                    }

                    Component.onCompleted: {
                        console.log("Id/sectionName", id, sectionName)
                    }
                }
            }
        }

        BasicText {
            Layout.fillWidth: true

            color: Style.textGreenColor
            verticalAlignment: Text.AlignTop
            font.capitalization: Font.AllUppercase
            text: getResourceName(currentResource)
        }
    }
}
