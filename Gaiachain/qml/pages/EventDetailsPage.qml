import QtQuick 2.11
import QtQuick.Layouts 1.1

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

BasePage {
    id: top
    property bool readOnly: true

    property string logId: ""

    Connections {
        target: pageManager
        // When using popup always add checking if I'm on top
        enabled: pageManager.isOnTop(page)
        onPopupAction: {
            switch(action) {
            case Enums.PopupAction.Save:
                console.log("Save action to implement")
                break
            case Enums.PopupAction.Exit:
                pageManager.backTo(pageManager.homePage())
                break
            case Enums.PopupAction.Cancel:
            default:
            }
        }
    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: s(Style.bigMargin)
            bottomMargin: 0
        }

        Flickable {
            id: flickableId
            Layout.fillHeight: true
            Layout.fillWidth: true

            clip: true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.OvershootBounds

            contentWidth: width
            contentHeight: contentLayout.implicitHeight

            Column {
                id: contentLayout
                spacing: s(Style.bigMargin)

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.companyName + ":")
                    contentText: "Harvest Inc." //TO_DO content texts are temporary
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.gpsHarvestLocation + ":")
                    contentText: "Harvest Inc. Location of harvest (GPS point): Location of harvest (GPS point):"
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.harvestDate + ":")
                    contentText: "Harvest Inc."
                }

                RowLayout {
                    width: flickableId.contentWidth

                    Items.TextWithTitle {
                        id: logTextId
                        Layout.preferredWidth: parent.width * 0.75

                        titleText: (Strings.logID + ":")
                        contentText: logId
                    }
                    Items.LayoutSpacer {}
                    Items.ImageButton {
                        Layout.preferredHeight: implicitWidth * 0.7
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        visible: !top.readOnly

                        source: Style.miniEditImgUrl
                        padding: s(10)

                        backgroundRadius: s(Style.smallMargin)
                        backgroundColor: Style.buttonGreyColor

                        onClicked: pageManager.back()
                    }
                    Items.LayoutSpacer {}
                }

                Items.TextWithTitle {
                    width: flickableId.contentWidth

                    titleText: (Strings.dateTimeDeparture + ":")
                    contentText: "Harvest Inc."
                }
            }
        }

        RowLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.preferredHeight: s(Style.footerHeight)
            visible: !top.readOnly

            spacing: s(Style.bigMargin)

            Items.LayoutSpacer{}
            Items.ImageButton
            {
                Layout.preferredWidth: s(Style.buttonHeight)
                Layout.preferredHeight: s(Style.buttonHeight)

                fillMode: Image.PreserveAspectFit

                backgroundColor: Style.buttonGreyColor
                source: Style.cancelImgUrl

                padding: s(22)

                onClicked: pageManager.enterPopup(Enums.Popup.Information, {
                                                      "text" : Strings.exitWithoutSaveQuestion,
                                                      "acceptButtonText": Strings.exit,
                                                      "rejectButtonText": Strings.cancel,
                                                      "acceptButtonType": Enums.PopupAction.Exit
                                                  }, true)
            }

            Items.ImageButton
            {
                Layout.preferredWidth: s(Style.buttonHeight)
                Layout.preferredHeight: s(Style.buttonHeight)

                fillMode: Image.PreserveAspectFit

                backgroundColor: Style.buttonGreyColor
                source: Style.okImgUrl

                padding: s(22)

                onClicked: pageManager.enterPopup(Enums.Popup.Information, {
                                                      "text" : Strings.saveQuestion,
                                                      "acceptButtonText": Strings.save,
                                                      "rejectButtonText": Strings.cancel,
                                                      "acceptButtonType": Enums.PopupAction.Save
                                                  })
            }
        }
    }
}
