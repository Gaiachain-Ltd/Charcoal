import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    readonly property bool headerIsEmpty: (headerText === Strings.empty)
    property bool highlighted: false

    property alias widget: mainLayout.data

    property alias headerText: header.text
    property alias headerTextColor: header.color

    property alias helpButtonVisible: image.visible
    property string helpTitle: headerText
    property string helpText: Strings.defaultHelp

    property alias separatorVisible: separatorLine.visible

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    Rectangle {
        id: backgroundHightlight
        color: GStyle.delegateHighlightColor
        visible: top.highlighted
        anchors.fill: parent
    }

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: headerIsEmpty ? GStyle.none : s(GStyle.smallMargin)

        RowLayout {
            Items.GText {
                id: header

                Layout.fillWidth: true

                font.bold: true
                horizontalAlignment: Text.AlignLeft

                visible: !headerIsEmpty
                wrapMode: Text.WordWrap
            }

            Image {
                id: image
                visible: false
                source: GStyle.helpButtonUrl

                MouseArea {
                    anchors.fill: parent
                    onClicked: pageManager.enter(Enums.Page.Help, {
                                                     "title": top.helpTitle,
                                                     "text": top.helpText
                                                 })
                }
            }
        }

        Rectangle {
            id: separatorLine
            Layout.fillWidth: true
            height: 1
            color: top.highlighted ? GStyle.fontHighlightColor : GStyle.inputBorderColor
            visible: false
        }
    }

    Rectangle {
        id: bottomLine
        Layout.fillWidth: true
        height: 1
        color: GStyle.fontHighlightColor
        visible: top.highlighted

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
    }
}
