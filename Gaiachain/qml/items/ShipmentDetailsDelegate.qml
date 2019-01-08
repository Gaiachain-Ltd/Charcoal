import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

ColumnLayout {
    id: top

    Layout.fillHeight: false

    property string placeType: type
    property var midYPos: []

    function enterEventDetailsPage() {
        pageManager.push(Enums.Page.EventDetails, {"readOnly": true})
    }

    onYChanged: {
        for (var i = 0; i < rep.count; ++i)
            rep.itemAt(i).updateMidYPos()
    }

    Repeater {
        id: rep
        model: attributes

        delegate: Item {
            id: delegateId

            Layout.fillWidth: true
            Layout.preferredHeight: mainLayout.implicitHeight

            function updateMidYPos() {
                midYPos[index] = mapToItem(mainRowLayout, 0, mainLayout.height / 2).y
            }

            onYChanged: updateMidYPos()
            Component.onCompleted: updateMidYPos()

            RowLayout {
                id: mainLayout

                anchors.left: parent.left
                anchors.right: parent.right

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0
                    BasicText {
                        Layout.fillWidth: true
                        Layout.preferredHeight: s(70)
                        horizontalAlignment: Text.AlignLeft
                        text: title
                    }
                    BasicText {
                        Layout.fillWidth: true
                        Layout.preferredHeight: s(70)
                        horizontalAlignment: Text.AlignLeft
                        font.bold: true
                        text: Strings.batch + " " + index
                    }
                    BasicText {
                        Layout.fillWidth: true
                        Layout.preferredHeight: s(70)
                        horizontalAlignment: Text.AlignLeft
                        text: contentText
                    }
                }

                ImageButton {
                    Layout.preferredWidth: s(Style.smallButtonHeight)
                    Layout.fillHeight: true

                    imageSize: s(Style.smallButtonHeight)

                    fillMode: Image.PreserveAspectFit
                    source: Style.rightArrowImgUrl
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: enterEventDetailsPage()
            }
        }
    }
}
