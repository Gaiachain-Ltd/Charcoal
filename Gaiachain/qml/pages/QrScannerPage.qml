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
                id: rectContainer
                anchors {
                    top: parent.top
                    topMargin: videoOutput.height * 0.25
                    left: parent.left
                    leftMargin: videoOutput.width * 0.2
                }
                width: videoOutput.width * 0.6
                height: width

                Rectangle
                {
                    anchors.fill: parent
                    border.width: 10
                    border.color: "white"
                    color: "transparent"
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
                    leftMargin: s(30)
                    rightMargin: s(30)
                    topMargin: s(15)
                    bottomMargin: s(15)
                }
                spacing: s(30)

                Items.BasicText
                {
                    text: scannedId.length === 0 ? Strings.scanning : scannedId
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: s(Style.bigPixelSize)
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Rectangle
                {
                    color: "grey"
                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

                Rectangle
                {
                    color: "grey"
                    opacity: 0.5
                    Layout.preferredWidth: s(Style.buttonHeight)
                    Layout.preferredHeight: s(Style.buttonHeight)
                }

            }
        }
    }

}
