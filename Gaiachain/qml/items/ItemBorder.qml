import QtQuick 2.11

import com.gaiachain.style 1.0

Item {    
    id: top
    readonly property int borderHeight: sr(10)
    readonly property int borderWidth: width * 0.25

    property bool error: false
    property bool finished: false
    property color borderColor: {
        if (error) {
            return GStyle.errorBorderColor
        } else if (finished) {
            return GStyle.finishedBorderColor
        } else {
            return GStyle.normalBorderColor
        }
    }

    Repeater
    {
        model: 8
        anchors.fill: parent
        Rectangle {
            id: rect

            color: borderColor

            // 0-1 = top-left
            // 2-3 = top-right
            // 4-5 = bottom-left
            // 6-7 = bottom-right
            readonly property bool isLeft: index <= 1 || index === 4 || index === 5
            readonly property bool isTop: index <= 3
            anchors {
                left: rect.isLeft ? parent.left : undefined
                right: rect.isLeft ? undefined : parent.right
                top: rect.isTop ? parent.top : undefined
                bottom: rect.isTop ? undefined : parent.bottom
            }

            width: index % 2 === 0 ? parent.borderWidth : parent.borderHeight
            height: index % 2 === 0 ? parent.borderHeight : parent.borderWidth
        }
    }
}
