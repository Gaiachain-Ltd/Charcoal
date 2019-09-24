import QtQuick 2.11
import QtQuick.Controls 2.5

import "../items" as Items

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

Items.BasePopup
{
    id: top
    buttons: [ { "text": Strings.ok, "action": Enums.PopupAction.Accept } ]
}
