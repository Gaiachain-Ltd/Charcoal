import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

ComboBox {
    id: top

    currentIndex: -1

    property alias placeholderText: input.placeholderText
    property string emptyListText: Strings.noEntries

    property bool optional: false
    readonly property bool isEmpty: currentIndex == -1

    property bool footerVisible: false
    property string footerText

    signal footerClicked

    function togglePopup() {
        top.popup.visible = !top.popup.visible
    }

    function clear() {
        currentIndex = -1
    }

    delegate: Items.GenericItemDelegate {
        width: ListView.view ? ListView.view.width : implicitWidth

        highlighted: (highlightedIndex === index)
        separatorVisible: (index !== 0)

        text: textRole ? (Array.isArray(model) ? modelData[textRole] : model[textRole]) : modelData
    }

    indicator: Items.GenericInput {
        id: input

        width: top.width

        placeholderText: enabled ? Strings.toSelect : Strings.empty

        iconEdge: Enums.Edge.RightEdge
        iconSource: optional && !isEmpty ? Style.clearImgUrl : Style.downArrowImgUrl
        enabled: top.enabled

        text: currentText
        readOnly: true

        iconItem.z: ma.z + 1 // this is to handle icon click firstly

        onIconClicked: {
            if (optional && !isEmpty) {
                top.clear()
            } else {
                top.togglePopup()
            }
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            enabled: top.model !== undefined

            onClicked: {
                forceActiveFocus()
                top.togglePopup()
            }
        }
    }

    background: Item {}

    popup: Popup {
        id: popup

        function footerClicked() {
            top.footerClicked()
        }

        y: top.height
        width: top.width

        focus: true

        onYChanged: {
            if (y !== 0 && Math.abs(y) < top.height) {
                height -= (top.height - Math.abs(y))
            }
        }

        onClosed: top.indicator.focus = false

        contentItem: ListView {
            id: entriesList
            implicitHeight: contentHeight

            clip: true
            currentIndex: top.highlightedIndex

            boundsBehavior: Flickable.StopAtBounds

            model: top.delegateModel

            header: Items.GenericItemDelegate {
                width: entriesList.width

                text: emptyListText
                visible: !entriesList.count
                height: visible ? implicitHeight : Style.none
            }
            footer: Items.GenericItemDelegate {
                text: footerText
                visible: footerVisible
                height: visible ? implicitHeight : Style.none

                bold: true
                separatorVisible: true

                onClicked: {
                    popup.footerClicked()
                    popup.close()
                }
            }
        }

        background: Rectangle {
            radius: sr(Style.tinyMargin)
            color: Style.backgroundColor
            border {
                width: sr(Style.separatorHeight)
                color: Style.separatorColor
            }
        }
    }
}
