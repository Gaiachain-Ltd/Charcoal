import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.GHeader {
    id: top

    property bool readOnly: false

    property var titles: []
    property var values: []
    property var icons: []

    readonly property bool isEmpty: (values === [])

    property int borderWidth: sr(1)

    function clear() {
        values = []
    }

    onTitlesChanged: {
        if (values.length === 0) {
            for (let title in titles) {
                values.push("")
            }
        }

        if (icons.length === 0) {
            for (let title in titles) {
                icons.push("")
            }
        }
    }
}
