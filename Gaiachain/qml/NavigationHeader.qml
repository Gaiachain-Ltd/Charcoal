import QtQuick 2.11
import QtQuick.Layouts 1.11

import "items" as Items

// TO_DO consider using Toolbar
Item {
    signal headerClicked()

    RowLayout {
        anchors.fill: parent

        //spacer
        Item {
            Layout.preferredWidth: s(20)
            Layout.fillHeight: true
        }

        Items.ImageButton {
            Layout.fillHeight: true
            Layout.preferredWidth: height

            source: "qrc:/ui/back"
        }

        Items.ImageItem {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            buttonWidth: height

            imageUrl: "qrc:/ui/timber"
            text: qsTr("Timber") //TO_DO set proper text
            textFont.pixelSize: s(60)
            textFont.capitalization: Font.AllUppercase
        }

        Items.ImageButton {
            Layout.fillHeight: true
            Layout.preferredWidth: height

            source: "qrc:/ui/home"
        }

        //spacer
        Item {
            Layout.preferredWidth: s(20)
            Layout.fillHeight: true
        }
    }
}
