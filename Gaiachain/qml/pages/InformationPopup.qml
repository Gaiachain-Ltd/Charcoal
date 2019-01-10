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
    property int acceptButtonType: Enums.PopupAction.Accept
    property string rejectButtonText: ""
    property int rejectButtonType: Enums.PopupAction.Cancel

    function cancelHandler() {
        pageManager.sendAction(rejectButtonType)
    }

    function acceptHandler() {
        pageManager.sendAction(acceptButtonType)
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
        width: parent.width * 0.8
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
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.6
                topPadding: s(Style.normalMargin)
                bottomPadding: s(Style.normalMargin)

                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: top.text
            }

            Rectangle
            {
                anchors.horizontalCenter: parent.horizontalCenter
                height: sr(3)
                width: parent.width

                color: Style.buttonGreyColor
            }

            Row
            {
                width: parent.width
                height: s(Style.buttonHeight)

                Items.ImageButton
                {
                    id: acceptButton

                    width: parent.width * (rejectButton.visible ? 0.5 : 1)
                    height: parent.height
                    visible: text != ""

                    text: top.acceptButtonText

                    backgroundColor: Style.pageBaseBackgroundColor
                    showIcon: false

                    onClicked: acceptHandler()
                }

                Rectangle
                {
                    width: sr(3)
                    height: parent.height
                    visible: acceptButton.visible && rejectButton.visible

                    color: Style.buttonGreyColor
                }

                Items.ImageButton
                {
                    id: rejectButton

                    width: parent.width * (acceptButton.visible ? 0.5 : 1)
                    height: parent.height
                    visible: text != ""

                    text: top.rejectButtonText || Strings.close

                    backgroundColor: Style.pageBaseBackgroundColor
                    showIcon: false

                    onClicked: cancelHandler()
                }
            }
        }
    }
}
