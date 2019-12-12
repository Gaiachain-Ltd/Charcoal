import QtQuick 2.11
import QtQuick.Layouts 1.1

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

BasePage {
    id: top

    readonly property int minImgScale: 1
    readonly property int maxImgScale: 3
    property int imageScale: 2

    readonly property point abidjanScale1Pos: Qt.point(1446, 1118)
    readonly property point abidjanScale2Pos: Qt.point(1320, 1099)
    readonly property point abidjanScale3Pos: Qt.point(1344, 1242)

    function increaseScale()
    {
        if(imageScale < maxImgScale)
        {
            ++imageScale
        }
    }

    function decreaseScale()
    {
        if(imageScale > minImgScale)
        {
            --imageScale
        }
    }

    function goAbidjan()
    {
        if(imageScale === 1) {
            flickable.contentX = abidjanScale1Pos.x
            flickable.contentY = abidjanScale1Pos.y
        } else if(imageScale === 2) {
            flickable.contentX = abidjanScale2Pos.x
            flickable.contentY = abidjanScale2Pos.y
        } else if(imageScale === 3) {
            flickable.contentX = abidjanScale3Pos.x
            flickable.contentY = abidjanScale3Pos.y
        }
    }

    Component {
        id: mapComponent

        Image
        {
            source: Style.abidjanMapTemplateImgUrl + imageScale
        }
    }

    Flickable
    {
        id: flickable

        anchors.fill: parent

        clip: true

        contentWidth: mapLoader.width
        contentHeight: mapLoader.height

        boundsBehavior: Flickable.StopAtBounds

        Loader {
            id: mapLoader

            asynchronous: true
            sourceComponent: mapComponent

            onLoaded: goAbidjan()
        }
    }

    RowLayout
    {
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            bottomMargin: s(Style.middleMargin)
            rightMargin: s(Style.middleMargin)
            leftMargin: s(Style.middleMargin)
        }

        spacing: s(Style.tinyMargin)

        Items.ImageButton
        {
            Layout.preferredHeight: s(Style.buttonHeight)
            Layout.preferredWidth: s(Style.buttonHeight)

            source: Style.homeImgBrownUrl

            onClicked: goAbidjan()
        }

        Items.LayoutSpacer {}

        Items.ImageButton
        {
            enabled: imageScale !== maxImgScale

            Layout.preferredHeight: s(Style.buttonHeight)
            Layout.preferredWidth: s(Style.buttonHeight)

            text: "+"
            textColor: enabled ? Style.textBrownColor : "#77AA6400"
            textPixelSize: s(Style.buttonPixelSize) * 2

            onClicked: increaseScale()
        }

        Items.ImageButton
        {
            enabled: imageScale !== minImgScale

            Layout.preferredHeight: s(Style.buttonHeight)
            Layout.preferredWidth: s(Style.buttonHeight)

            text: "-"
            textColor: enabled ? Style.textBrownColor : "#77AA6400"
            textPixelSize: s(Style.buttonPixelSize) * 2

            onClicked: decreaseScale()
        }
    }
}
