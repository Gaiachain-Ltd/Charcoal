import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../items" as Items

Item {
    id: top

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    property int margin: s(GStyle.middleMargin)

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: s(GStyle.middleMargin)

        Items.GText {
            Layout.fillWidth: true
            Layout.bottomMargin: margin

            font.pixelSize: s(GStyle.titlePixelSize)
            font.bold: true

            text: mainController.application.name + " v. " + mainController.application.version
        }

        Items.LayoutSeparator {
        }

        Items.GText {
            Layout.fillWidth: true
            Layout.topMargin: margin
            Layout.bottomMargin: margin

            font.pixelSize: s(GStyle.titlePixelSize)
            font.bold: true

            text: "delphineahoussi@gmail.com"
        }

        Items.LayoutSeparator {
        }

        MouseArea {
            id: row

            readonly property QtObject manager: mainController.languageManager
            property int currentIndex: manager.currentLanguageIndex

            Layout.preferredHeight: 60
            Layout.fillWidth: true
            Layout.topMargin: margin
            Layout.bottomMargin: margin

            onClicked: {
                console.log("Language switch clicked")
                let newIndex = row.currentIndex + 1;
                if (newIndex >= row.manager.languages.length) {
                    newIndex = 0;
                }

                row.manager.currentLanguageIndex = newIndex;
            }

            Row {
                anchors {
                    centerIn: parent
                }

                height: parent.height
                spacing: s(GStyle.buttonLetterSpacing)

                Image {
                    anchors.verticalCenter: parent.verticalCenter

                    source: row.manager.languages[row.currentIndex].icon
                }

                Items.GText {
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }
                    Layout.fillWidth: true
                    font.pixelSize: s(GStyle.subtitlePixelSize)
                    text: row.manager.languages[row.currentIndex].language
                }
            }
        }

        Items.LayoutSeparator {
        }

        Items.SvgImage
        {
            id: logoMilo

            Layout.fillWidth: true
            Layout.margins: s(GStyle.bigMargin)

            height: s(GStyle.logoHeight)
            source: GStyle.logoMiloAboutUrl
        }

    }
}
