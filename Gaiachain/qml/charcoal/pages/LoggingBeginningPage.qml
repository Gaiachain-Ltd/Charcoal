import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    title: Strings.loggingBeginning

    Component.onCompleted: refreshData()

    function refreshData() {
        //sessionManager.getUnusedLotIds()
    }

    Headers.ComboBoxHeader {
        id: parcelComboBox
        Layout.fillWidth: true
        headerText: Strings.parcel
        helpButtonVisible: true
        helpText: Strings.loggingBeginningParcelHelp
    }

    Headers.InputHeader {
        id: repsIdInputHeader
        Layout.fillWidth: true
        headerText: Strings.malebiRepsId
        helpButtonVisible: true
        helpText: Strings.loggingBeginningRepsIdHelp
    }

    Headers.ComboBoxHeader {
        id: villageComboBox
        Layout.fillWidth: true
        headerText: Strings.village
        helpButtonVisible: true
        helpText: Strings.loggingBeginningVillageHelp
    }

    Headers.ComboBoxHeader {
        id: treeSpeciesComboBox
        Layout.fillWidth: true
        headerText: Strings.treeSpecies
        helpButtonVisible: true
        helpText: Strings.loggingBeginningTreeSpeciesHelp
    }

    Headers.InputDateHeader {
        id: beginningDateHeader
        Layout.fillWidth: true
        headerText: Strings.beginningDate
        helpButtonVisible: true
        helpText: Strings.loggingBeginningDateHelp
    }

    Common.PositionSourceHandler {
        id: gpsSource

        function errorMessage() {
            if (noAccess) {
                return Strings.enableGpsLocation
            } else if (!valid) {
                return Strings.gpsNotAvailable
            } else if (!positioningSupported) {
                return Strings.gpsTurnedOff
            } else {
                return Strings.gpsInvalid
            }
        }
    }

    Headers.ButtonInputHeader {
        id: gpsCoordinatesButtonInputHeader
        helpButtonVisible: true
        helpText: Strings.loggingBeginningGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}
