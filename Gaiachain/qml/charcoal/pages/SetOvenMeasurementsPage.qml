import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.items 1.0

import "../../items" as Items
import "../../pages" as Pages

Pages.GPage {
    id: root

    title: Strings.setOvenMeasurements

    property bool validPageData: widthInput.text.length > 0
                                 && lengthInput.text.length > 0
                                 && height1Input.text.length > 0
                                 && height2Input.text.length > 0
    property alias proceedButtonVisible: proceedButton.visible
    property alias proceedButtonEnabled: proceedButton.enabled
    property alias proceedButtonText: proceedButton.text

    property var ovenDimensions: []
    property int backToPage: Enums.Page.InvalidPage

    function proceed() {
        pageManager.backTo(backToPage, {
                               "ovenDimensions": ovenDimensions
                           })
    }

    function backToHomeHandler() {
        pageManager.back()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: s(GStyle.mediumMargin)

        Flickable {
            id: flickable

            Layout.fillWidth: true
            Layout.fillHeight: true

            contentHeight: content.height

            clip: true

            boundsBehavior: Flickable.StopAtBounds

            onHeightChanged: {
                // To avoid overlapping inputs by keyboard
                var desiredVisibleY = Qt.inputMethod.cursorRectangle.y + Qt.inputMethod.cursorRectangle.height + s(GStyle.hugeMargin)
                var realVisibleY = mapToGlobal(0, y + height).y

                if (desiredVisibleY > realVisibleY) {
                    contentY += desiredVisibleY - realVisibleY
                }
            }

            Column {
                id: content
                width: flickable.width
                padding: s(GStyle.bigMargin)
                spacing: s(GStyle.smallMargin)

                GridLayout {
                    width: content.width
                    columns: 2
                    columnSpacing: s(GStyle.middleMargin)
                    rowSpacing: columnSpacing

                    Items.GInput {
                        id: widthInput
                        Layout.fillWidth: true
                        placeholderText: Strings.widthPlaceholder
                        onActiveFocusChanged: if (activeFocus) image.selectedDimension = TrapezoidOvenItem.Width
                    }

                    Items.GInput {
                        id: lengthInput
                        Layout.fillWidth: true
                        placeholderText: Strings.lengthPlaceholder
                        onActiveFocusChanged: if (activeFocus) image.selectedDimension = TrapezoidOvenItem.Length
                    }

                    Items.GInput {
                        id: height1Input
                        Layout.fillWidth: true
                        placeholderText: Strings.height1Placeholder
                        onActiveFocusChanged: if (activeFocus) image.selectedDimension = TrapezoidOvenItem.Height1
                    }

                    Items.GInput {
                        id: height2Input
                        Layout.fillWidth: true
                        placeholderText: Strings.height2Placeholder
                        onActiveFocusChanged: if (activeFocus) image.selectedDimension = TrapezoidOvenItem.Height2
                    }
                }

                TrapezoidOvenItem {
                    id: image
                    width: flickable.width
                    height: 300
                }
            }
        }

        Items.GButton {
            id: proceedButton

            Layout.leftMargin: s(GStyle.hugeMargin)
            Layout.rightMargin: Layout.leftMargin
            Layout.bottomMargin: Layout.leftMargin
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            padding: s(GStyle.bigMargin)

            enabled: root.validPageData

            text: Strings.proceed

            onClicked: top.proceed()
        }
    }
}
