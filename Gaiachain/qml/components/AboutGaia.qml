import QtQuick 2.14
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
        spacing: s(Style.tinyMargin)

        Items.BasicText {
            Layout.fillWidth: true

            font {
                pixelSize: s(Style.titlePixelSize)
                bold: true
            }

            text: mainController.application.name
        }

        Items.BasicText {
            Layout.fillWidth: true

            font.pixelSize: s(Style.subtitlePixelSize)

            text: "v%1:%2".arg(mainController.application.version)
                          .arg(mainController.application.commit)
        }

        Items.LayoutSeparator {

        }

        AboutGaiaDependency {
            Layout.fillWidth: true
        }
    }
}
