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

    property color borderColor: Style.inputBorderColor

    property alias placeholderText: textArea.placeholderText
    property alias inputText: textArea.text

    radius: s(Style.tinyMargin)
    border {
        width: sr(Style.controlDefaultBorderWidth)
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
                pixelSize: s(Style.inputPixelSize)
                family: Style.primaryFontFamily
            }

            placeholderTextColor: Style.textPrimaryColor
            padding: s(Style.bigMargin)

            wrapMode: TextArea.Wrap
        }

        ScrollBar.vertical: ScrollBar {}
    }
}
