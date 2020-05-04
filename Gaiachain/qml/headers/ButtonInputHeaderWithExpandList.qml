import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../headers" as Headers
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
        spacing: GStyle.none

        ButtonInputHeader {
            id: inputHeader
            Layout.fillWidth: true

            property bool expanded: false

            headerText: top.headerText
            inputText: top.inputText

            showIcon: expandList.count
            iconSource: expanded ? GStyle.expandUpImgUrl : GStyle.expandDownImgUrl

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

            implicitHeight: GStyle.none
            opacity: GStyle.hidden

            interactive: false

            spacing: s(GStyle.bigMargin)

            delegate: Headers.InputHeader {
                width: parent.width

                showIcon: false
                readOnly: true
                color: GStyle.headerBackgroundColor
                headerText: headerValue
                inputText: inputValue
                suffixText: inputSuffixValue
            }

            Rectangle {
                id: packagesLine

                property real additionalSpace: GStyle.none

                anchors {
                    top: parent.top
                    topMargin: -additionalSpace

                    bottom: parent.bottom
                    bottomMargin: s(GStyle.tinyMargin)

                    horizontalCenter: parent.horizontalCenter
                }

                width: sr(GStyle.separatorHeight)
                color: GStyle.separatorColor

                z: parent.z - 1
            }

            ParallelAnimation {
                id: expandingAnimation

                NumberAnimation {
                    target: expandList
                    property: "opacity"
                    to: inputHeader.expanded ? GStyle.hidden : GStyle.visible
                    duration: GStyle.animationDuration
                }

                NumberAnimation {
                    target: expandList
                    property: "implicitHeight"
                    to: inputHeader.expanded ? GStyle.none : expandList.contentHeight
                    duration: GStyle.animationDuration
                }

                NumberAnimation {
                    target: mainLayout
                    property: "spacing"
                    to: inputHeader.expanded ? GStyle.none : s(GStyle.hugeMargin)
                    duration: GStyle.animationDuration
                }
                NumberAnimation {
                    target: packagesLine
                    property: "additionalSpace"
                    to: inputHeader.expanded ? GStyle.none : s(GStyle.hugeMargin)
                    duration: GStyle.animationDuration
                }
            }
        }
    }
}
