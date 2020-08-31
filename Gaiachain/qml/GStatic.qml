pragma Singleton

import QtQuick 2.12

QtObject {
    // qr scanner
    readonly property int frameSvgImgHeight: 550
    readonly property int checkSvgImageHeight: 250
    readonly property real normalizedScanSize: 0.6 // Should be between 0 and 1
    readonly property string qrInputMask: "NNNN-NNNN-NNNN"
    readonly property string qrSectionInputMask: "NNNN"

    // gps tracking
    readonly property int gpsRefreshInterval: 1000 // ms

    // internet connection
    readonly property int internetCheckConnectionTimeInterval: 10000
}
