import QtQuick 2.11
import QtQuick.Layouts 1.1

import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    ColumnLayout {
        anchors {
            fill: parent
            margins: s(Style.normalMargin)
        }

        Flickable {
            Layout.fillHeight: true
            Layout.fillWidth: true

            clip: true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.OvershootBounds

            contentWidth: width
            contentHeight: mainLayout.implicitHeight

            ColumnLayout {
                id: mainLayout
                spacing: s(Style.normalMargin)

                Items.TextWithTitle {
                    Layout.fillWidth: true

                    titleText: "Company name:"
                    contentText: "Harvest Inc."
                }

                Items.TextWithTitle {
                    Layout.fillWidth: true

                    titleText: "Location of harvest (GPS point):"
                    contentText: "Harvest Inc. Location of harvest (GPS point): Location of harvest (GPS point):"
                }

                Items.TextWithTitle {
                    Layout.fillWidth: true

                    titleText: "Date of harvest:"
                    contentText: "Harvest Inc."
                }

                RowLayout {
                    Layout.fillWidth: true
                    Items.TextWithTitle {
                        id: logTextId
                        Layout.fillWidth: true

                        titleText: "Log ID:"
                        contentText: "Harvest Inc."
                    }

                    Items.ImageButton {
                        Layout.preferredWidth: s(Style.smallButtonHeight)
                        Layout.preferredHeight: Math.min(s(Style.smallButtonHeight), logTextId.implicitHeight)
                        source: Style.miniEditImgUrl
                    }
                }

                Items.TextWithTitle {
                    Layout.fillWidth: true

                    titleText: "Date and time of departure:"
                    contentText: "Harvest Inc."
                }

                Items.TextWithTitle {
                    Layout.fillWidth: true

                    titleText: "Date and time of departure:"
                    contentText: "Harvest Inc."
                }

                Items.TextWithTitle {
                    Layout.fillWidth: true

                    titleText: "Date and time of departure:"
                    contentText: "Harvest Inc."
                }
            }

        }




    }

}
