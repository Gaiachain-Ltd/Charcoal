import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

GPage {
    id: top

    default property alias pageContent: contentLayout.data
    property bool validPageData: true

    property alias proceedButtonEnabled: proceedButton.enabled
    property alias proceedButtonText: proceedButton.text

    function proceed() { // redefined in supply chain pages
    }

    function backToHomeHandler() {
        pageManager.openPopup(Enums.Popup.Confirm, { "text": Strings.askForExit }, "EXIT_CONFIRM")
    }

    function createSummaryItem(header, value, inputIconSource = "", suffix = "") {
        return {
          "headerValue": header,
          "value": value,
          "inputIconSource": inputIconSource.toString(),
          "suffixValue": suffix
        }
    }

    Connections {
        target: pageManager
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            if (popupId != "EXIT_CONFIRM") {
                return
            }

            switch (action) {
            case Enums.PopupAction.Accept:
                pageManager.backTo(pageManager.homePage())
                break
            default:
                break
            }
        }
    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: s(GStyle.hugeMargin)
        }

        spacing: s(GStyle.bigMargin)

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true

            contentHeight: contentLayout.implicitHeight

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

            ColumnLayout {
                id: contentLayout
                anchors.fill: parent

                spacing: s(GStyle.smallMargin)
            }
        }

        Items.GButton {
            id: proceedButton

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            padding: s(GStyle.bigMargin)

            enabled: top.validPageData

            text: Strings.proceed

            onClicked: top.proceed()
        }
    }
}
