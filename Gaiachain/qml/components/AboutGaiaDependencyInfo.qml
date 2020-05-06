import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import com.gaiachain.style 1.0

import "../items" as Items

Item {
    id: top

    property alias infoVersion: version.text
    property alias infoLicense: license.text

    property string infoLicenseUrl

    readonly property var urlIsValid: function (url) {
        let matches = url.match(/^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$/)
        return (matches !== null && matches.length !== 0)
    }

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: s(GStyle.tinyMargin)

        Items.GText {
            id: version

            Layout.fillWidth: true
        }

        Items.GText {
            id: license

            Layout.fillWidth: true

            MouseArea {
                anchors.fill: parent
                enabled: top.urlIsValid(top.infoLicenseUrl)
                onClicked: Qt.openUrlExternally(top.infoLicenseUrl)
            }
        }
    }
}
