import QtQuick 2.12
import QtQuick.Controls 2.4

import com.gaiachain.style 1.0

ImageButton {
    id: top

    property real numberRadius: s(Style.buttonNumberRadius)
    property int number: 0

    topPadding: numberRadius
    rightPadding: numberRadius

    Rectangle {
        anchors {
            top: parent.top
            topMargin: -(top.numberRadius - border.width)
            right: parent.right
            rightMargin: -(top.numberRadius - border.width)
        }

        width: 2 * top.numberRadius
        height: 2 * top.numberRadius

        radius: top.numberRadius

        color: palette.buttonText

        border {
            width: sr(Style.controlBoldestBorderWidth)
            color: palette.button
        }

        visible: number != 0

        BasicText {
            anchors {
                fill: parent
                margins: s(Style.tinyMargin) / 3
            }

            font.bold: true

            fontSizeMode: Text.Fit
            minimumPixelSize: s(Style.tinyPixelSize)

            color: palette.button

            text: top.number
        }
    }
}
