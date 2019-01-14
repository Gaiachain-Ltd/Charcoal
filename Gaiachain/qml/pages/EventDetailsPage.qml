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
            margins: s(Style.bigMargin)
            bottomMargin: 0
        }

        Flickable {
            id: flickableId
            Layout.fillHeight: true
            Layout.fillWidth: true

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

                        titleText: (Strings.logID + ":")
                        contentText: attributes.shipmentId
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
    }
}
