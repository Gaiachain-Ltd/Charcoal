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

    signal next()
    signal previous()
    signal returnToCurrent()
    signal headerClicked()

    RowLayout {
        id: mainLayout

        anchors {
            fill: parent
            margins: s(Style.middleMargin)
        }

        spacing: s(Style.smallMargin)

        Items.BasicText {
            Layout.alignment: Qt.AlignVCenter

            horizontalAlignment: Text.AlignLeft
            text: Helper.getMonthName(currentMonth) + " " + currentYear
            font.weight: Font.DemiBold
            font.pixelSize: s(Style.subtitlePixelSize)

            MouseArea {
                anchors.fill: parent
                onClicked: top.headerClicked()
            }
        }

        Items.ImageButton {
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)
            Layout.preferredWidth: s(Style.buttonImageSmallHeight)

            visible: returnButton

            palette.button: Style.calendarArrowButtonColor
            source: Style.returnCurrentMonthImgUrl
            padding: s(Style.tinyMargin)

            onClicked: top.returnToCurrent()
        }

        Items.LayoutSpacer { Layout.fillWidth: true }

        Items.ImageButton {
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)
            Layout.preferredWidth: s(Style.buttonImageSmallHeight)

            palette.button: Style.calendarArrowButtonColor
            source: Style.leftArrowImgUrl
            padding: s(Style.tinyMargin)

            onClicked: top.previous()
        }

        Items.ImageButton {
            Layout.preferredHeight: s(Style.buttonImageSmallHeight)
            Layout.preferredWidth: s(Style.buttonImageSmallHeight)

            palette.button: Style.calendarArrowButtonColor
            source: Style.rightArrowImgUrl
            padding: s(Style.tinyMargin)

            onClicked: top.next()
        }
    }
}
