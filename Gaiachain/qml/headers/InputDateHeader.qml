import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components
import "../popups" as Popups

Headers.GHeader {
    id: top

    property var selectedDate: optional ? undefined : currentDate

    property date currentDate: new Date

    property alias placeholderText: input.placeholderText

    property bool optional: false
    readonly property bool isEmpty: selectedDate === undefined

    property bool readOnly: false

    property QtObject popup: Popups.CalendarPopup {
        currentDate: top.currentDate
        selectedDate: top.selectedDate
        onClosed: top.selectedDate = selectedDate
    }

    function togglePopup() {
        popup.visible = !popup.visible
    }

    function clear() {
        selectedDate = undefined
    }

    widget: Items.GInput {
        id: input

        Layout.fillWidth: true
        Layout.leftMargin: top.margins
        Layout.rightMargin: top.margins

        text: isEmpty ? Strings.empty : top.selectedDate.toLocaleDateString(Qt.locale(), Strings.dateFormat)

        readOnly: true

        iconEdge: Enums.Edge.RightEdge
        iconSource: optional && !isEmpty ? GStyle.clearImgUrl : GStyle.calendarButtonImgUrl

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
            onClicked: {
                if (readOnly == false) {
                    forceActiveFocus()
                    top.togglePopup()
                }
            }
        }
    }


}
