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

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: Style.none

        Items.InputHeader {
            id: inputHeader

            property bool showRelatedPackages: false

            Layout.fillWidth: true

            onShowRelatedPackagesChanged: relatedPackagesListVisibleAnimation.start()

            readOnly: true
            headerText: top.headerText
            inputText: top.inputText

            showIcon: !expandList.empty()
            iconSource: showRelatedPackages ? Style.expandUpImgUrl : Style.expandDownImgUrl

            MouseArea {
                anchors.fill: parent
                onClicked: inputHeader.showRelatedPackages = !inputHeader.showRelatedPackages
                enabled: !expandList.empty()
            }
        }

        ListView {
            id: expandList

            function empty() {
                return (count === 0)
            }

            Layout.fillWidth: true

            implicitHeight: Style.none
            opacity: Style.hidden

            interactive: false

            spacing: s(Style.bigMargin)

            delegate: Items.InputHeader {
                width: parent.width

                readOnly: true
                color: Style.headerBackgroundColor
                headerText: headerValue
                inputText: inputValue
                suffixText: inputSuffixValue
            }

            ParallelAnimation {
                id: relatedPackagesListVisibleAnimation

                NumberAnimation {
                    target: expandList
                    property: "opacity"
                    to: inputHeader.showRelatedPackages ? Style.hidden : Style.visible
                    duration: Style.animationDuration
                }

                NumberAnimation {
                    target: expandList
                    property: "implicitHeight"
                    to: inputHeader.showRelatedPackages ? Style.none : expandList.contentHeight
                    duration: Style.animationDuration
                }

                NumberAnimation {
                    target: mainLayout
                    property: "spacing"
                    to: inputHeader.showRelatedPackages ? Style.none : s(Style.hugeMargin)
                    duration: Style.animationDuration
                }
            }
        }
    }
}
