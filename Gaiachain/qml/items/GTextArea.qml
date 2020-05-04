import QtQuick 2.11
import QtQuick.Controls 2.5

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

/* TODO try to make it working more Qt-way
 * https://doc-snapshots.qt.io/qt5-5.9/qtquickcontrols2-customize.html#customizing-textarea
 * https://doc-snapshots.qt.io/qt5-5.9/qml-qtquick-controls2-textarea.html#scrollable-textarea
 */
Rectangle {
    id: top

    property color borderColor: GStyle.inputBorderColor

    property alias placeholderText: textArea.placeholderText
    property alias inputText: textArea.text

    radius: s(GStyle.tinyMargin)
    border {
        width: sr(GStyle.controlDefaultBorderWidth)
        color: top.borderColor
    }

    Flickable {
        anchors.fill: parent

        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        contentHeight: textArea.implicitHeight

        TextArea.flickable: TextArea {
            id: textArea

            font {
                pixelSize: s(GStyle.inputPixelSize)
                family: GStyle.primaryFontFamily
            }

            placeholderTextColor: GStyle.textPrimaryColor
            padding: s(GStyle.bigMargin)

            wrapMode: TextArea.Wrap
        }

        ScrollBar.vertical: ScrollBar {}
    }
}
