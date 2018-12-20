/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtMultimedia 5.5

import BarcodeCaptureFilter 1.0

ColumnLayout {
    anchors.fill: parent

    VideoOutput {
        id: cameraOutput
        source: camera
        focus: visible // to receive focus and capture key events when visible
        Layout.fillWidth: true
        Layout.fillHeight: true
        fillMode: VideoOutput.Stretch
        autoOrientation: true
        filters: [ barcodeCaptureFilter ] // add barcode capture filter

        Camera {
            id: camera

            focus {
                focusMode: Camera.FocusContinuous
                focusPointMode: Camera.FocusPointCenter
            }
        }

        MBarcodeCaptureFilter {
            id: barcodeCaptureFilter

            sourceWidth: cameraOutput.width
            sourceHeight: cameraOutput.height
            orientation: cameraOutput.orientation

            onTagFound: {
                console.log("Barcode found and decoded: %1".arg(tag))
                barcodeResultText.visible = true
                barcodeResultText.text = tag
                timer.restart()
            }

            Component.onCompleted: {
                if (typeof requestDrawRect !== "undefined")
                    barcodeCaptureFilter.requestDrawRect.connect(drawRect)
                if (typeof requestDrawRect !== "undefined")
                    barcodeCaptureFilter.requestCleanView.connect(cleanView)
            }

            function drawRect(rect) {
                console.log("Draw rect", rect)
                openCvOverlay.rect = rect
                openCvOverlay.requestPaint()
            }

            function cleanView() {
                console.log("Clean view")
                openCvOverlay.rect = null
                openCvOverlay.requestPaint()
            }
        }

        Canvas {
            id: openCvOverlay
            anchors.fill: parent

            property var rect: null

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                if (rect == null)
                    return

                ctx.strokeStyle = "green"
                ctx.lineWidth = 10
                ctx.strokeRect(rect.x, rect.y, rect.width, rect.height)
            }
        }
    }

    Text {
        id: barcodeResultText
        color: "blue"
        visible: false
        Layout.fillWidth: true
        Layout.preferredHeight: 200
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Timer {
        id: timer
        interval: 5000

        onTriggered: {
            barcodeResultText.visible = false
        }
    }
}

