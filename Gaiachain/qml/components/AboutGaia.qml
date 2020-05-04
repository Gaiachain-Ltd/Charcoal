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

        ListView {
            readonly property QtObject controller: mainController.languageController
            model: controller.languages
            delegate: ItemDelegate {
                text: modelData.language
                icon.source: modelData.icon
                onClicked: controller.currentLanguageIndex = index
            }
        }
    }
}
