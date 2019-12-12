import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0

BasePopup
{
    id: top
    buttons: [ { "text": Strings.ok, "action": Enums.PopupAction.Accept , "Action": Enums.PopupAction.Accept },
               { "text": Strings.cancel, "action": Enums.PopupAction.Cancel , "Action": Enums.PopupAction.Cancel } ]
}
