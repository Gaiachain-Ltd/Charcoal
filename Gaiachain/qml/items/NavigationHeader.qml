import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

// TO_DO consider using Toolbar
Item {
    signal headerClicked()

    property var currentResource: Enums.CommodityType.Timber //TO_DO set proper text after user set

    function getResourceName(res) {
        switch(res) {
        case Enums.CommodityType.Timber: return Strings.timber
        case Enums.CommodityType.Charcoal: return Strings.charcoal
        case Enums.CommodityType.Cocoa: return Strings.cocoa
        default:
            console.warn("Invalid resource type!")
        }

        return Strings.timber + "!" // Add "!" if invalid
    }

    RowLayout {
        anchors.fill: parent

        LayoutSpacer { spacerWidth: s(20) }

        ImageButton {
            Layout.fillHeight: true
            Layout.preferredWidth: height

            padding: s(40)
            fillMode: Image.PreserveAspectFit
            source: Style.backImgUrl

            onClicked: pageManager.pop()
        }

        ImageItem {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            buttonWidth: height * 0.8

            imageUrl: Style.timberImgUrl

            text: getResourceName(currentResource)
            textColor: Style.textGreenColor
            textFont.pixelSize: s(50)
            textFont.capitalization: Font.AllUppercase
        }

        ImageButton {
            Layout.fillHeight: true
            Layout.preferredWidth: height

            padding: s(40)
            fillMode: Image.PreserveAspectFit
            source: Style.homeImgUrl

            onClicked: pageManager.goToInitial()
        }

        LayoutSpacer { spacerWidth: s(20) }
    }
}
