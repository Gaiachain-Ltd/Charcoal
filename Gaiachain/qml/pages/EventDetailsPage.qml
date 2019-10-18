import QtQuick 2.11
import QtQuick.Layouts 1.1

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

BasePage {
    id: top
    property bool editable: false
    property var attributes: ({})

    signal editClicked()
    signal cancelClicked()
    signal acceptClicked()

    ColumnLayout {
        anchors {
            fill: parent
            bottomMargin: 0
        }
        spacing: 0

        Flickable {
            id: flickableId
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.leftMargin: s(Style.bigMargin) * 0.5
            Layout.rightMargin: s(Style.bigMargin) * 0.5

            clip: true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.OvershootBounds

            contentWidth: width
            contentHeight: contentLayout.implicitHeight

            Column {
                id: contentLayout
                spacing: s(Style.bigMargin)

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.companyName + ":")
                    contentText: attributes.company
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.gpsLocationOf.arg(Helpers.placeTypeToString(attributes.place).toLowerCase()) + ":")
                    contentText: Helpers.formatLocation(attributes.location)
                }

                RowLayout {
                    width: flickableId.contentWidth

                    Items.TextWithTitle {
                        id: commodityTextId
                        Layout.preferredWidth: parent.width * 0.75

                        titleText: Helpers.getCurrentIdText() + ":"
                        contentText: attributes.packageId
                    }
                    Items.LayoutSpacer {}
                    Items.ImageButton {
                        Layout.preferredHeight: implicitWidth * 0.7
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        visible: top.editable

                        source: Style.miniEditImgUrl
                        padding: s(10)

                        backgroundRadius: s(Style.smallMargin)
                        backgroundColor: Style.buttonGreyColor

                        onClicked: editClicked()
                    }
                    Items.LayoutSpacer {}
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: Helpers.placeActionToDateTimeString(attributes.action) + ":"
                    contentText: Helpers.convertTimestampToDate(attributes.timestamp)
                }
            }
        }

        RowLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.preferredHeight: s(Style.footerHeight)
            Layout.leftMargin: s(Style.bigMargin) * 0.5
            Layout.rightMargin: s(Style.bigMargin) * 0.5
            visible: top.editable

            spacing: s(Style.bigMargin)

            Items.LayoutSpacer{}
            Items.ImageButton
            {
                Layout.preferredWidth: s(Style.buttonHeight)
                Layout.preferredHeight: s(Style.buttonHeight)

                fillMode: Image.PreserveAspectFit

                backgroundColor: Style.buttonGreyColor
                source: Style.cancelImgUrl

                padding: s(22)

                onClicked: cancelClicked()
            }

            Items.ImageButton
            {
                Layout.preferredWidth: s(Style.buttonHeight)
                Layout.preferredHeight: s(Style.buttonHeight)

                fillMode: Image.PreserveAspectFit

                backgroundColor: Style.buttonGreyColor
                source: Style.okImgUrl

                padding: s(22)

                onClicked: acceptClicked()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: top.height * 0.2

            visible: userManager.commodityType === Enums.CommodityType.Cocoa

            color: Style.forestBackgroundColor

            RowLayout {
                spacing: parent.width * 0.05
                anchors {
                   fill: parent
                   margins: parent.width * 0.1
                }

                Item {
                    Layout.preferredWidth: parent.width * 0.25
                    Layout.fillHeight: true

                    Items.BasicText {
                        anchors.centerIn: parent

                        color: Style.textSecondaryColor
                        textFormat: Text.RichText
                        font {
                            weight: Font.Black
                            pixelSize: s(Style.pixelSize) * 1.25
                        }
                        text: "GLOBAL<br><font color='#474747'>FOREST</font><br>WATCH"
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Items.BasicText {
                        anchors.centerIn: parent

                        color: Style.textSecondaryColor
                        text: Strings.zeroDeforestation
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: arrow.width * 1.5
                    Items.SvgImage {
                        id: arrow
                        height: s(Style.headerArrowHeight)
                        width: s(Style.headerArrowWidth)
                        anchors.centerIn: parent
                        source: Style.rightArrowWhiteImgUrl
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: pageManager.enter(Enums.Page.Map)
            }
        }
    }
}
