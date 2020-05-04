import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.style 1.0

import "../../items" as Items

Item {
    id: top

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    property int currentMonth
    property int currentYear

    property bool returnButton: true

    signal next()
    signal previous()
    signal returnToCurrent()
    signal headerClicked()

    RowLayout {
        id: mainLayout

        anchors {
            fill: parent
            margins: s(GStyle.middleMargin)
        }

        spacing: s(GStyle.smallMargin)

        Items.GText {
            Layout.alignment: Qt.AlignVCenter

            horizontalAlignment: Text.AlignLeft
            text: Helper.getMonthName(currentMonth) + " " + currentYear
            font.weight: Font.DemiBold
            font.pixelSize: s(GStyle.subtitlePixelSize)

            MouseArea {
                anchors.fill: parent
                onClicked: top.headerClicked()
            }
        }

        Items.ImageButton {
            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)
            Layout.preferredWidth: s(GStyle.buttonImageSmallHeight)

            visible: returnButton

            palette.button: GStyle.calendarArrowButtonColor
            source: GStyle.returnCurrentMonthImgUrl
            padding: s(GStyle.tinyMargin)

            onClicked: top.returnToCurrent()
        }

        Items.LayoutSpacer { Layout.fillWidth: true }

        Items.ImageButton {
            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)
            Layout.preferredWidth: s(GStyle.buttonImageSmallHeight)

            palette.button: GStyle.calendarArrowButtonColor
            source: GStyle.leftBlackArrowImgUrl
            padding: s(GStyle.tinyMargin)

            onClicked: top.previous()
        }

        Items.ImageButton {
            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)
            Layout.preferredWidth: s(GStyle.buttonImageSmallHeight)

            palette.button: GStyle.calendarArrowButtonColor
            source: GStyle.rightBlackArrowImgUrl
            padding: s(GStyle.tinyMargin)

            onClicked: top.next()
        }
    }
}
