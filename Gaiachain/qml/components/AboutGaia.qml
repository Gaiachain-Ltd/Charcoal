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

            text: Qt.application.name
        }

        Items.BasicText {
            Layout.fillWidth: true

            font.pixelSize: s(Style.subtitlePixelSize)

            text: "v%1".arg(Qt.application.version)
        }

        Items.LayoutSeparator {

        }

        AboutGaiaDependency {
            Layout.fillWidth: true
        }
    }
}
