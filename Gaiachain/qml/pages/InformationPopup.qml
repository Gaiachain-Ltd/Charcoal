import QtQuick 2.11

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Item
{
    id: top
    property int page: Enums.Page.InvalidPage
    property bool isPopup: true

    property string text: ""
    property string acceptButtonText: ""
    property string rejectButtonText: ""

    function cancelHandler() {
        pageManager.back(true)
    }

    function acceptHandler() {
        console.log("Empty function called!")
    }

    Rectangle
    {
        anchors.fill: parent
        color: Style.backgroundShadowColor
        MouseArea
        {
            anchors.fill: parent
            onClicked: {}
            onPressed: {}
        }
    }

    Rectangle
    {
        id: rect
        anchors.centerIn: parent
        width: parent.width * 0.5
        height: contentColumn.height

        color: Style.pageBaseBackgroundColor

        radius: s(Style.buttonRadius)

        layer.enabled: true

        Column
        {
            id: contentColumn
            anchors.centerIn: parent
            width: parent.width

            Items.BasicText
            {
                text: top.text
                width: parent.width * 0.95
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                height: paintedHeight * 2
            }

            Rectangle
            {
                height: sr(1)
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                color: Style.buttonGreyColor
            }

            Row
            {
                width: parent.width
                height: s(Style.buttonHeight)

                Items.ImageButton
                {
                    id: acceptButton
                    text: top.acceptButtonText
                    backgroundColor: Style.pageBaseBackgroundColor
                    visible: text != ""
                    width: parent.width * (rejectButton.visible ? 0.5 : 1)
                    height: parent.height
                    showIcon: false

                    onClicked: acceptHandler()
                }

                Rectangle
                {
                    width: sr(1)
                    height: parent.height
                    color: Style.buttonGreyColor
                    visible: !acceptButton.visible || !rejectButton.visible
                }

                Items.ImageButton
                {
                    id: rejectButton
                    text: top.rejectButtonText || Strings.close
                    visible: text != ""
                    backgroundColor: Style.pageBaseBackgroundColor
                    width: parent.width * (acceptButton.visible ? 0.5 : 1)
                    height: parent.height
                    showIcon: false

                    onClicked: cancelHandler()
                }
            }
        }
    }
}
