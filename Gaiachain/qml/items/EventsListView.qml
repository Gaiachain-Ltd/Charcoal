import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

Item {
    id: top

    property alias viewModel: mainView.model
    property alias delegateHeight: mainView.delegateHeight
    property alias backgroundColor: background.color

    property bool displayDate: true

    signal delegateClicked(var data)

    Rectangle {
        id: background
        anchors.fill: parent

        color: "transparent"
    }

    ListView {
        id: mainView

        anchors {
            fill: parent
            topMargin: mainView.spacing
        }
        clip: true

        property real delegateHeight: s(50)

        spacing: s(Style.smallMargin)

        delegate: Item {
            id: delegate
            height: Math.max(ListView.view.delegateHeight, mainLayout.childrenRect.height)
            width: parent.width - s(Style.bigMargin) * 3

            anchors.horizontalCenter: parent.horizontalCenter

            readonly property bool isLast: (ListView.view.count - 1) === index

            ColumnLayout {
                id: mainLayout

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top

                spacing: mainView.spacing

                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    spacing: s(Style.smallMargin)

                    BasicText {
                        id: text

                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        bottomPadding: s(25)
                        topPadding: s(25)

                        horizontalAlignment: Text.AlignLeft

                        wrapMode: Text.WordWrap
                        textFormat: Text.RichText
                        elide: Text.ElideNone

                        readonly property string actionStr: Helpers.placeActionToStringAction(Number(action)).toLowerCase()
                        text: displayDate ? Strings.eventText.arg(Helpers.convertTimestampToDate(timestamp)).arg(shipmentId).arg(actionStr).arg(Helpers.placeTypeToString(Number(place)).toLowerCase()).arg(company)
                                          : Strings.eventTextCalendar.arg(shipmentId).arg(actionStr).arg(Helpers.placeTypeToString(Number(place)).toLowerCase()).arg(company)
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

                    visible: !delegate.isLast

                    color: Style.listEventsDelegateBottomLineColor
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: delegateClicked({ "shipmentId": shipmentId })
            }
        }
    }
}
