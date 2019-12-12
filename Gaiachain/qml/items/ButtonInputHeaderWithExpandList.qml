import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

Item {
    id: top

    property alias model: expandList.model
    property alias headerText: inputHeader.headerText
    property alias inputText: inputHeader.inputText
    property alias color: inputHeader.color
    property alias expanded: inputHeader.expanded

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: Style.none

        Items.ButtonInputHeader {
            id: inputHeader
            Layout.fillWidth: true

            property bool expanded: false

            headerText: top.headerText
            inputText: top.inputText

            showIcon: expandList.count
            iconSource: expanded ? Style.expandUpImgUrl : Style.expandDownImgUrl

            onExpandedChanged: expandingAnimation.start()

            onClicked: {
                if (expandList.count) {
                    inputHeader.expanded = !inputHeader.expanded
                }
            }
        }

        ListView {
            id: expandList
            Layout.fillWidth: true

            implicitHeight: Style.none
            opacity: Style.hidden

            interactive: false

            spacing: s(Style.bigMargin)

            delegate: Items.InputHeader {
                width: parent.width

                showIcon: false
                readOnly: true
                color: Style.headerBackgroundColor
                headerText: headerValue
                inputText: inputValue
                suffixText: inputSuffixValue
            }

            Rectangle {
                id: packagesLine

                property real additionalSpace: Style.none

                anchors {
                    top: parent.top
                    topMargin: -additionalSpace

                    bottom: parent.bottom
                    bottomMargin: s(Style.tinyMargin)

                    horizontalCenter: parent.horizontalCenter
                }

                width: sr(Style.separatorHeight)
                color: Style.separatorColor

                z: parent.z - 1
            }

            ParallelAnimation {
                id: expandingAnimation

                NumberAnimation {
                    target: expandList
                    property: "opacity"
                    to: inputHeader.expanded ? Style.hidden : Style.visible
                    duration: Style.animationDuration
                }

                NumberAnimation {
                    target: expandList
                    property: "implicitHeight"
                    to: inputHeader.expanded ? Style.none : expandList.contentHeight
                    duration: Style.animationDuration
                }

                NumberAnimation {
                    target: mainLayout
                    property: "spacing"
                    to: inputHeader.expanded ? Style.none : s(Style.hugeMargin)
                    duration: Style.animationDuration
                }
                NumberAnimation {
                    target: packagesLine
                    property: "additionalSpace"
                    to: inputHeader.expanded ? Style.none : s(Style.hugeMargin)
                    duration: Style.animationDuration
                }
            }
        }
    }
}
