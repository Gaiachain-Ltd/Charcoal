import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

// TO_DO consider using Toolbar
Item {
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

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            LayoutSpacer { spacerWidth: s(20) }

            ImageButton {
                Layout.fillHeight: true
                Layout.preferredWidth: height

                padding: s(40)
                fillMode: Image.PreserveAspectFit
                source: Style.backImgUrl

                onClicked: pageManager.pop()
            }

            LayoutSpacer { spacerWidth: s(10) }

            ListView {
                Layout.fillHeight: true
                orientation: ListView.Vertical

                interactive: false

                model: ListModel {
                    ListElement {
                        sourceUrl: Style.miniCalendarImgUrl
                    }
                    ListElement {
                        sourceUrl: Style.timelineImgUrl
                    }
                    ListElement {
                        sourceUrl: Style.detailsImgUrl
                    }
                }

                delegate: Item {
                    height: ListView.height
                    width: delegateLayout.childrenRect.width

                    RowLayout {
                        id: delegateLayout
                        anchors.fill: parent

                        ImageButton {
                            Layout.fillHeight: true
                            Layout.preferredWidth: height

                            padding: s(30)
                            fillMode: Image.PreserveAspectFit
                            source: Style.homeImgUrl

                            onClicked: callback()
                        }

                        SvgImage {
                            visible: (ListView.view.count - 1) !== index
                            Layout.fillHeight: true
                            Layout.preferredWidth: s(20)

                            fillMode: Image.PreserveAspectFit
                            source: Style.rightArrowImgUrl
                        }
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
