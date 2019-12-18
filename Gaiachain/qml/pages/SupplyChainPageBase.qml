import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
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
            margins: s(Style.hugeMargin)
        }

        spacing: s(Style.bigMargin)

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true

            contentHeight: contentLayout.implicitHeight

            clip: true

            boundsBehavior: Flickable.StopAtBounds

            onHeightChanged: {
                // To avoid overlapping inputs by keyboard
                var desiredVisibleY = Qt.inputMethod.cursorRectangle.y + Qt.inputMethod.cursorRectangle.height + s(Style.hugeMargin)
                var realVisibleY = mapToGlobal(0, y + height).y

                if (desiredVisibleY > realVisibleY) {
                    contentY += desiredVisibleY - realVisibleY
                }
            }

            ColumnLayout {
                id: contentLayout
                anchors.fill: parent

                spacing: s(Style.smallMargin)
            }
        }

        Items.GenericButton {
            id: proceedButton

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            padding: s(Style.bigMargin)

            enabled: top.validPageData

            text: Strings.proceed

            onClicked: top.proceed()
        }
    }
}
