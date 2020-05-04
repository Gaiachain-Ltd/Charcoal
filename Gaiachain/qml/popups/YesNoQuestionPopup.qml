import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

import "../../popups" as Popups

Popups.ButtonPopup
{
    id: top
    buttons: [
        { "text": Strings.yes, "action": Enums.PopupAction.Yes },
        { "text": Strings.no, "action": Enums.PopupAction.No }
    ]
}
