import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

//import "../items" as Items
//import "../headers" as Headers
//import "../components" as Components
import "../../pages" as Pages

Pages.GPage {
    id: root

    property var urls: []

    onUrlsChanged: console.log("Urls are:", urls)

    backgroundColor: GStyle.photoGalleryBackgroundColor

    ListView {
        anchors.fill: parent
        spacing: 5 * s(GStyle.bigMargin)

        model: urls

        delegate: Image {
            width: root.width
            source: "file://" + urls[index]
            fillMode: Image.PreserveAspectFit
        }
    }
}
