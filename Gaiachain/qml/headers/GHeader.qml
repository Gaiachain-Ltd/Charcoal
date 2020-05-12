import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

Item {
    id: top

    readonly property bool headerIsEmpty: (headerText === Strings.empty)

    // "Flat" summary style
    property bool summaryMode: false
    // Background color of the whole control
    property color backgroundColor: GStyle.backgroundColor
    // Color of main header text
    property color mainColor: GStyle.textPrimaryColor
    // Color of second line of text and separator lines
    property color secondaryColor: GStyle.textPrimaryColor
    property bool highlighted: false

    property alias widget: mainLayout.data

    property alias headerText: header.text
    property alias headerTextColor: header.color

    property alias helpButtonVisible: image.visible
    property string helpTitle: headerText
    property string helpText: Strings.defaultHelp

    property int margins: s(GStyle.hugeMargin)

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    Rectangle {
        id: backgroundHightlight
        color: top.backgroundColor
        visible: top.highlighted
        anchors.fill: parent
    }

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: headerIsEmpty ? GStyle.none : s(GStyle.tinyMargin)

        RowLayout {
            Layout.topMargin: top.isFirstItem? top.margins : 0
            Layout.leftMargin: top.margins
            Layout.rightMargin: top.margins

            Items.GText {
                id: header

                Layout.fillWidth: true

                font.bold: true
                color: top.mainColor
                font.capitalization: top.highlighted? Font.AllUppercase : Font.MixedCase
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
            Layout.leftMargin: top.margins
            Layout.rightMargin: top.margins

            id: separatorLine
            Layout.fillWidth: true
            height: 1
            color: top.secondaryColor
            visible: top.summaryMode
        }
    }

    Rectangle {
        id: bottomLine
        Layout.fillWidth: true
        height: 1
        color: top.secondaryColor
        visible: top.highlighted

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
    }
}
