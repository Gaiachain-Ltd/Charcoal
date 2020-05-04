import QtQuick 2.12
import QtQuick.Controls 2.4

import com.gaiachain.style 1.0

ImageButton {
    id: top

    property real numberRadius: s(GStyle.buttonNumberRadius)
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
            width: sr(GStyle.controlBoldestBorderWidth)
            color: palette.button
        }

        visible: number != 0

        GText {
            anchors {
                fill: parent
                margins: s(GStyle.tinyMargin) / 3
            }

            font {
                bold: true
                letterSpacing: 0   // required to display properly
            }

            fontSizeMode: Text.Fit
            minimumPixelSize: s(GStyle.tinyPixelSize)

            color: palette.button

            text: top.number
        }
    }
}
