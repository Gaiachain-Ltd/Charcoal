import QtQuick 2.12

import com.gaiachain.style 1.0

SequentialAnimation {
    id: top

    property QtObject target

    property string property
    property int duration: Style.animationDuration

    property alias outValue: outAnimation.to
    property alias inValue: inAnimation.to

    NumberAnimation {
        id: outAnimation

        target: top.target
        property: top.property
        duration: top.duration
        easing.type: Style.animationEasing
    }

    PropertyAnimation {}

    NumberAnimation {
        id: inAnimation

        target: top.target
        property: top.property
        duration: top.duration
        easing.type: Style.animationEasing
    }
}
