import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

import "../items" as Items

BasePage {
    id: top

    header {
        title: Strings.gaiachain
        logoVisible: true
    }

    ColumnLayout {
        anchors.fill: parent

        Items.LayoutSpacer {}
    }
}
