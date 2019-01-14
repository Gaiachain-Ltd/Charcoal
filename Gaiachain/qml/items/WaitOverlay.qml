import QtQuick 2.11

import QtQuick.Layouts 1.11

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Rectangle
{
    color: Style.backgroundColor

    visible: false

    property bool logoVisible: false

    Column
    {
        anchors.fill: parent

        Item
        {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height * 0.5

            visible: logoVisible

            SvgImage
            {
                anchors.centerIn: parent

                source: Style.logoImgUrl

                readonly property int calcWidth: parent.width * 0.7
                width: calcWidth
                height: 0.35 * calcWidth
            }
        }

        Item
        {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height * (logoVisible ? 0.5 : 1.0)

            SvgImage {
                id: spinner
                anchors.centerIn: parent

                source: Style.spinnerImgUrl

                RotationAnimator {
                    target: spinner
                    from: 0
                    to: 360
                    duration: 1000
                    running: true
                    loops: Animation.Infinite
                }
            }
        }
    }
}
