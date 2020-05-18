import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.enums 1.0

import "../../headers" as Headers
import "../../popups" as Popups

Headers.InputDateHeader {
    id: top
    helpButtonVisible: true

    popup: Popups.CalendarFullscreenPagePopup {
        currentDate: top.currentDate
        selectedDate: top.selectedDate
        onClosed: top.selectedDate = selectedDate
    }
}
