import QtQuick 2.11

import QtQuick.Controls 2.4

import com.gaiachain.enums 1.0

Item {
    property int page: Enums.Page.InvalidPage
    property bool isPopup: false

    property var attachedStackView: StackView.view

    function closeEventHandler()
    {
        return false
    }
}
