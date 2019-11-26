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

    delegate: ItemDelegate {
        id: itemDelegate

        implicitWidth: indicatorInput.width
        implicitHeight: indicatorInput.implicitHeight

        background: Rectangle {
            anchors.fill: parent

            color: itemDelegate.highlighted ? Style.delegateHighlightColor : Style.backgroundColor

            Rectangle {
                anchors.top: parent.top

                width: parent.width
                height: sr(Style.separatorHeight)

                visible: (index !== 0)
                color: Style.separatorColor
            }
        }

        contentItem: Items.BasicText {
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            font.bold: itemDelegate.highlighted

            text: textRole ? (Array.isArray(model) ? modelData[textRole] : model[textRole]) : modelData
        }

        highlighted: (highlightedIndex === index)
    }

    indicator: Items.GenericInput {
        id: indicatorInput

        width: top.width

        placeholderText: enabled ? Strings.toSelect : Strings.empty

        iconEdge: Enums.Edge.RightEdge
        iconSource: Style.downArrowImgUrl
        enabled: top.enabled

        text: currentText

        MouseArea {
            anchors.fill: parent
            onClicked: if (top.model !== undefined && top.model.count !== 0) top.popup.visible = !top.popup.visible
        }
    }

    background: Item {}

    popup: Popup {
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

            ScrollBar.vertical: ScrollBar {
                active: true
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
