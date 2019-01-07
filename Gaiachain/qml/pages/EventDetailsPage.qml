import QtQuick 2.11
import QtQuick.Layouts 1.1

import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    ColumnLayout {
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
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
                spacing: s(Style.normalMargin)

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.companyName + ":")
                    contentText: "Harvest Inc." //TO_DO content texts are temporary
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.gpsHarvestLocation + ":")
                    contentText: "Harvest Inc. Location of harvest (GPS point): Location of harvest (GPS point):"
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.harvestDate + ":")
                    contentText: "Harvest Inc."
                }

                RowLayout {
                    width: flickableId.contentWidth

                    Items.TextWithTitle {
                        id: logTextId
                        Layout.preferredWidth: parent.width * 0.75

                        titleText: (Strings.logID + ":")
                        contentText: "Harvest Inc."
                    }
                    Items.LayoutSpacer {}
                    Items.ImageButton {
                        Layout.preferredHeight: implicitWidth * 0.7
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                        source: Style.miniEditImgUrl
                        padding: s(10)

                        backgroundRadius: s(15)
                        backgroundColor: Style.buttonGreyColor

                        onClicked: {
                            console.log("Show popup!")
                        }
                    }
                    Items.LayoutSpacer {}
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.dateTimeDeparture + ":")
                    contentText: "Harvest Inc."
                }
            }
        }
    }
}
