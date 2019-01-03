import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtMultimedia 5.9
import QZXing 2.3

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

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

            Item
            {
                anchors {
                    top: parent.top
                    topMargin: videoOutput.height * 0.25
                    left: parent.left
                    leftMargin: videoOutput.width * 0.2
                }
                width: videoOutput.width * 0.6
                height: width

                readonly property int borderHeight: sr(10)
                readonly property int borderWidth: width * 0.25

                // top - left
                Rectangle
                {
                    anchors {
                        left: parent.left
                        top: parent.top
                    }

                    width: parent.borderWidth
                    height: parent.borderHeight
                }
                Rectangle
                {
                    anchors {
                        left: parent.left
                        top: parent.top
                    }

                    height: parent.borderWidth
                    width: parent.borderHeight
                }

                // top - right
                Rectangle
                {
                    anchors {
                        right: parent.right
                        top: parent.top
                    }

                    width: parent.borderWidth
                    height: parent.borderHeight
                }
                Rectangle
                {
                    anchors {
                        right: parent.right
                        top: parent.top
                    }

                    height: parent.borderWidth
                    width: parent.borderHeight
                }

                // bottom - left
                Rectangle
                {
                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                    }

                    width: parent.borderWidth
                    height: parent.borderHeight
                }
                Rectangle
                {
                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                    }

                    height: parent.borderWidth
                    width: parent.borderHeight
                }

                // bottom - right
                Rectangle
                {
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }

                    width: parent.borderWidth
                    height: parent.borderHeight
                }
                Rectangle
                {
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }

                    height: parent.borderWidth
                    width: parent.borderHeight
                }
            }

            Items.ImageButton
            {
                onClicked: pageManager.back()

                backgroundColor: "#99000000"
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

                    backgroundColor: Style.greyButtonColor
                    source: Style.keyboardImgUrl

                    padding: s(Style.smallMargin)

                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

                Items.ImageButton
                {
                    onClicked: console.warn("Accepting code not implemented!")

                    fillMode: Image.PreserveAspectFit

                    enabled: scannedId.length > 0

                    backgroundColor: enabled ? Style.greyButtonColor : "#E6E6E6"
                    source: Style.loginImgUrl

                    padding: s(Style.smallMargin)

                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

            }
        }
    }

}
