import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

FocusScope
{
    id: top

    property alias text: inputItem.text
    property alias source: image.source
    property alias input: inputItem

    property alias showImage: image.visible
    property alias background: rect

    property alias placeholderText: placeholder.text
    property alias placeholder: placeholder

    property bool isPassword: false

    property int additionalInputMethodHints: 0

    property Item nextInput: null

    signal moveToNextInput()

    onMoveToNextInput: {
        if (nextInput !== null)
            nextInput.focus = true
    }

    Rectangle
    {
        id: rect
        anchors.fill: parent
        radius: s(Style.smallMargin)
        border {
            width: sr(1)
            color: Style.buttonGreyColor
        }
        RowLayout
        {
            id: row
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                margins: s(Style.normalMargin)
            }

            spacing: s(Style.smallMargin)

            SvgImage {
                id: image
                Layout.fillHeight: true
                Layout.preferredWidth: row.height
                Layout.alignment: Qt.AlignVCenter
                visible: false
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: inputItem.height

                layer.enabled: true

                // Placed inside item to enable clipping.
                TextInput
                {
                    id: inputItem

                    focus: true

                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        right: parent.right
                    }

                    autoScroll: true

                    maximumLength: 50

                    font {
                        pixelSize: s(Style.pixelSize-5)
                        family: Style.primaryFontFamily
                    }
                    echoMode: isPassword ? TextInput.Password : TextInput.Normal

                    inputMethodHints: Qt.ImhNoPredictiveText | additionalInputMethodHints

                    Keys.onReturnPressed: moveToNextInput()
                    Keys.onEnterPressed: moveToNextInput()
                    Keys.onTabPressed: moveToNextInput()

                    BasicText
                    {
                        id: placeholder

                        anchors {
                            left: parent.left
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }

                        color: Style.textGreyColor

                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignLeft

                        font: inputItem.font
                        opacity: inputItem.text.length == 0 ? 1 : 0

                        Behavior on opacity { PropertyAnimation { duration: 50 } }
                    }
                }
            }

        }
    }
}

