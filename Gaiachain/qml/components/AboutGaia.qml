import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0

import "../items" as Items

Item {
    id: top

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: s(GStyle.tinyMargin)

        Items.GText {
            Layout.fillWidth: true

            font {
                pixelSize: s(GStyle.titlePixelSize)
                bold: true
            }

            text: mainController.application.name
        }

        Items.GText {
            Layout.fillWidth: true

            font.pixelSize: s(GStyle.subtitlePixelSize)

            text: "v%1:%2".arg(mainController.application.version)
                          .arg(mainController.application.commit)
        }

        Items.LayoutSeparator {

        }

        AboutGaiaDependency {
            Layout.fillWidth: true
        }

        MouseArea {
            id: row

            readonly property QtObject manager: mainController.languageManager
            property int currentIndex: manager.currentLanguageIndex

            Layout.preferredHeight: 60
            Layout.fillWidth: true

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
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

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
    }
}
