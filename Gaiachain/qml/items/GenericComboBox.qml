import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items

ComboBox {
    id: top

    currentIndex: -1

    property alias placeholderText: indicatorInput.placeholderText

    property bool optional: false
    readonly property bool isEmpty: currentIndex == -1

    property bool footerVisible: false
    property string footerText

    signal footerClicked

    function togglePopup() {
        popup.visible = !popup.visible
    }

    function clear() {
        currentIndex = -1
    }

    delegate: Items.GenericItemDelegate {
        highlighted: (highlightedIndex === index)
        separatorVisible: (index !== 0)

        text: textRole ? (Array.isArray(model) ? modelData[textRole] : model[textRole]) : modelData
    }

    indicator: Items.GenericInput {
        id: indicatorInput

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
            enabled: top.model !== undefined && top.model.rowCount() > 0

            onClicked: {
                forceActiveFocus()
                top.togglePopup()
            }
        }
    }

    background: Item {}

    popup: Popup {
        id: comboBoxPopup

        y: top.height
        width: top.width

        onYChanged: {
            if (y !== 0 && Math.abs(y) < top.height) {
                height -= (top.height - Math.abs(y))
            }
        }

        focus: true

        contentItem: ListView {
            implicitHeight: contentHeight

            clip: true
            currentIndex: top.highlightedIndex

            boundsBehavior: Flickable.StopAtBounds

            model: top.delegateModel

            footer: Items.GenericItemDelegate {
                text: footerText
                visible: footerVisible
                height: footerVisible ? implicitHeight : Style.none

                onClicked: {
                    comboBoxPopup.parent.footerClicked()
                    comboBoxPopup.parent.popup.visible = false
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
