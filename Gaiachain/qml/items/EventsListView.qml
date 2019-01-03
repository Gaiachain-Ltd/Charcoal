import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0

Item {
    id: top

    property alias viewModel: mainView.model
    property alias delegateHeight: mainView.delegateHeight
    property alias backgroundColor: background.color

    signal delegateClicked(var data)

    Rectangle {
        id: background
        anchors.fill: parent

        color: "transparent"
    }

    ListView {
        id: mainView

        anchors.fill: parent
        clip: true

        property real delegateHeight: s(50)

        delegate: Item {
            height: Math.max(ListView.view.delegateHeight, mainLayout.childrenRect.height)
            width: utility.proportionalWidth(Style.listEventsDelegateRelativeHeight)

            anchors.horizontalCenter: parent.horizontalCenter

            ColumnLayout {
                id: mainLayout

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top

                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    spacing: s(30)

                    BasicText {
                        id: text

                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        bottomPadding: s(25)
                        topPadding: s(25)

                        horizontalAlignment: Text.AlignLeft

                        wrapMode: Text.WordWrap
                        elide: Text.ElideNone
                        text: "TEST fjhjkf ffjdkf d gfldf jdlfkljfkl d f fd fklklfds"
                    }

                    SvgImage {
                        id: image

                        Layout.fillHeight: true
                        Layout.preferredWidth: s(20)

                        fillMode: Image.PreserveAspectFit
                        source: Style.rightArrowImgUrl
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: sr(1)

                    color: Style.listEventsDelegateBottomLineColor
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: delegateClicked({"idx": index})
            }
        }
    }
}
