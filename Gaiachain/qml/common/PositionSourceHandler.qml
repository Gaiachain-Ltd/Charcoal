import QtQuick 2.12
import QtPositioning 5.12

import com.gaiachain.static 1.0

Item {
    readonly property bool validCoordinate: coordinate !== undefined
    property var coordinate: positionSourceLoader.positionReady &&
                             positionSourceLoader.item.validCoordinate ? positionSourceLoader.item.position.coordinate
                                                                       : undefined
    property bool valid: (positionSourceLoader.item ? positionSourceLoader.item.valid : false)
    property bool positioningSupported: (positionSourceLoader.item && positionSourceLoader.item.supportedPositioningMethods !== PositionSource.NoPositioningMethods)
    property bool noAccess: (positionSourceLoader.item && positionSourceLoader.item.sourceError === PositionSource.AccessError)

    function update() {
        if (positionSourceLoader.positionReady) {
            positionSourceLoader.item.update()
        }
    }

    /* Bare PositionSource seems not to work properly in some cases when switching GPS on / off.
     * It updates part of properties but not all and do not provide a valid coordinates.
     * Calling start / stop doesn't help, only reloading of the whole component.
     * Thus the Loader.
     */
    Loader {
        id: positionSourceLoader

        readonly property bool positionReady: status === Loader.Ready && item
        active: true

        function refreshDelayed(delay = Static.gpsRefreshInterval) {
            active = false

            refreshTimer.interval = delay
            refreshTimer.start()
        }
        function refreshNow() {
            refreshDelayed(0)
        }

        Timer {
            id: refreshTimer

            repeat: false
            onTriggered: positionSourceLoader.active = true
        }

        sourceComponent: Component {
            PositionSource {
                readonly property bool dataAvailable: valid &&
                                                      (supportedPositioningMethods !== PositionSource.NoPositioningMethods)
                readonly property bool validCoordinate: dataAvailable &&
                                                        position.latitudeValid &&
                                                        position.longitudeValid

                function checkStatus() {
                    if (dataAvailable &&
                            sourceError != PositionSource.NoError) {
                        positionSourceLoader.refreshNow()
                    } else if (valid &&
                               (supportedPositioningMethods === PositionSource.NoPositioningMethods)) {
                        positionSourceLoader.refreshDelayed()
                    }
                }

                active: true
                updateInterval: Static.gpsRefreshInterval

                onSupportedPositioningMethodsChanged: checkStatus()
                Component.onCompleted: checkStatus()
            }
        }
    }
}
