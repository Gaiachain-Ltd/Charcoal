import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

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

        ImageButton {
            Layout.fillHeight: true
            Layout.preferredWidth: height

            source: Style.backImgUrl
        }

        ImageItem {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            buttonWidth: height

            imageUrl: Style.timberImgUrl
            text: Strings.timber //TO_DO set proper text
            textFont.pixelSize: s(60)
            textFont.capitalization: Font.AllUppercase
        }

        ImageButton {
            Layout.fillHeight: true
            Layout.preferredWidth: height

            source: Style.homeImgUrl
        }

        //spacer
        Item {
            Layout.preferredWidth: s(20)
            Layout.fillHeight: true
        }
    }
}
