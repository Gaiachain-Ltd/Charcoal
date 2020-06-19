import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.style 1.0

import "../items" as Items

Item {
    id: top

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    property int currentMonth
    property int currentYear

    property bool returnButton: true
    property bool sparseLayout: false

    property int fontCapitalization: Font.MixedCase

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

        Items.ImageButton {
            id: previousSparse

            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)
            Layout.preferredWidth: s(GStyle.buttonImageSmallHeight)

            palette.button: GStyle.calendarArrowButtonColor
            source: GStyle.leftBlackArrowImgUrl
            padding: s(GStyle.tinyMargin)
            visible: sparseLayout

            onClicked: top.previous()
        }

        Items.LayoutSpacer {
            Layout.fillWidth: true
            visible: sparseLayout
        }

        Items.GText {
            Layout.alignment: Qt.AlignVCenter

            horizontalAlignment: Text.AlignLeft
            text: Helper.getMonthName(currentMonth) + " " + currentYear
            font.weight: Font.DemiBold
            font.pixelSize: s(GStyle.subtitlePixelSize)
            font.capitalization: top.fontCapitalization

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
            id: previousTight

            Layout.preferredHeight: s(GStyle.buttonImageSmallHeight)
            Layout.preferredWidth: s(GStyle.buttonImageSmallHeight)

            palette.button: GStyle.calendarArrowButtonColor
            source: GStyle.leftBlackArrowImgUrl
            padding: s(GStyle.tinyMargin)
            visible: !sparseLayout

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
