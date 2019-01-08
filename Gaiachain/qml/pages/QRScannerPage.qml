import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtMultimedia 5.9
import QZXing 2.3

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

/*
        Jak zeskanuje poprawny kod, to ramka zrobi się zielona (#96c354),
        a jak błędny, to czerwona (ff0000) i pojawi się napis "Scan failed",
        tak jak jest teraz, tylko czerwony.

        Oprócz tej zmiany w obydwu przypadkach (poprawny i niepoprawny kod)
        kamera zrobi zdjęcie zeskanowanego kodu i je wyświetli,
        a obok przycisku [ x ] (anuluj/zamknij),
        pojawi się nowa ikona do ponownego odpalenia trybu skanowania.
  */

BasePage {
    id: top

    function enterEventDetailsPage() {
        pageManager.enter(Enums.Page.EventDetails, {"readOnly": false})
    }

    property string scannedId: ""

    Connections
    {
        target: pageManager
        onPop: camera.stop()
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0

        Item {
            id: cameraContainer
            Layout.fillHeight: true
            Layout.fillWidth: true

            Camera {
                id: camera
            }

            VideoOutput {
                id: videoOutput
                anchors.fill: parent
                source: camera
                filters: [zxingFilter]
                fillMode: VideoOutput.PreserveAspectCrop
                autoOrientation: true
            }

            Items.ItemBorder
            {
                anchors.centerIn: parent
                width: Math.min(videoOutput.width, videoOutput.height) * 0.6
                height: width
            }

            Items.ImageButton
            {
                onClicked: pageManager.back()

                backgroundColor: Style.backgroundShadowColor
                source: Style.cancelImgUrl

                fillMode: Image.PreserveAspectFit

                padding: s(Style.smallMargin) * 1.5

                anchors {
                    top: parent.top
                    topMargin: s(Style.bigMargin)
                    right: parent.right
                    rightMargin: s(Style.bigMargin)
                }
            }

            QZXingFilter {
                id: zxingFilter
                decoder {
                    enabledDecoders: QZXing.DecoderFormat_QR_CODE

                    onTagFound:  {
                        if (scannedId.length === 0) {
                            if (utility.parseInt(tag) > 0) {
                                scannedId = tag
                            } else {
                                console.warn("Wrong code content!", tag)
                            }
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: s(Style.footerHeight)

            RowLayout
            {
                anchors {
                    fill: parent
                    leftMargin: s(Style.normalMargin)
                    rightMargin: s(Style.normalMargin)
                    topMargin: s(Style.smallMargin)
                    bottomMargin: s(Style.smallMargin)
                }
                spacing: s(Style.bigMargin)

                Items.BasicText
                {
                    text: scannedId.length === 0 ? Strings.scanning : String("%1: <b>%2</b>").arg(Strings.id).arg(scannedId)
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: s(Style.bigPixelSize)
                    textFormat: Text.RichText
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Items.ImageButton
                {
                    onClicked: console.warn("Input by keyboard not implemented!")

                    fillMode: Image.PreserveAspectFit

                    backgroundColor: Style.buttonGreyColor
                    source: Style.keyboardImgUrl

                    padding: s(Style.smallMargin)

                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

                Items.ImageButton
                {
                    onClicked: top.enterEventDetailsPage()

                    fillMode: Image.PreserveAspectFit

                    //enabled: scannedId.length > 0

                    backgroundColor: enabled ? Style.buttonGreyColor : "#E6E6E6"
                    source: Style.loginImgUrl

                    padding: s(Style.smallMargin)

                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

            }
        }
    }

}
