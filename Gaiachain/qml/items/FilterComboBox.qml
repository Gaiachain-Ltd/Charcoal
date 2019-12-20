import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

GenericComboBox {
    id: top

    property alias sourceModel: keywordFilter.sourceModel

    model: KeywordFilterProxyModel {
        id: keywordFilter
        filterRole: ModelHelper.roleNameToNumber(top.textRole, sourceModel)
    }

    popup: Popup {
        id: popup

        function footerClicked() {
            top.footerClicked()
        }

        palette: top.palette

        parent: Overlay.overlay

        margins: s(Style.bigMargin)

        x: margins
        y: margins
        width: parent.width - 2 * margins
        height: parent.height - 2 * margins - (Qt.inputMethod.keyboardRectangle.y > 0 ? Qt.inputMethod.keyboardRectangle.height : 0)

        Behavior on height { NumberAnimation { duration: Style.keyboardAnimationDuration } }

        focus: true

        Overlay.modal: Rectangle {
            color: Style.backgroundShadowColor
        }

        onClosed: top.indicator.focus = false

        contentItem: ColumnLayout {
            RowLayout {
                Layout.fillHeight: false

                spacing: s(Style.smallSpacing)

                Items.GenericInput {
                    id: searchInput
                    Layout.fillWidth: true

                    placeholderText: Strings.search

                    onTextChanged: keywordFilter.keyword = text
                }
                PureImageButton {
                    Layout.preferredHeight: s(Style.buttonImageTinyHeight)

                    source: Style.closeBlackImgUrl
                    onClicked: popup.close()
                }
            }

            ListView {
                id: entriesList
                Layout.fillHeight: true
                Layout.fillWidth: true

                clip: true
                cacheBuffer: 0  // it produces UI issues on filtering
                currentIndex: top.highlightedIndex

                boundsBehavior: Flickable.StopAtBounds

                model: delegateModel

                ScrollBar.vertical: ScrollBar {}

                header: Items.GenericItemDelegate {
                    width: entriesList.width

                    text: emptyListText
                    visible: !entriesList.count
                    height: visible ? implicitHeight : Style.none
                }

                footerPositioning: ListView.OverlayFooter
                footer: Items.GenericItemDelegate {
                    width: entriesList.width

                    text: footerText
                    visible: footerVisible
                    height: visible ? implicitHeight : Style.none

                    separatorVisible: true

                    z: 3    // https://doc.qt.io/qt-5/qml-qtquick-listview.html#stacking-order-in-listview - to be above delegates

                    onClicked: popup.footerClicked()
                }
            }

            Component.onCompleted: searchInput.forceActiveFocus()
        }

        background: Rectangle {
            radius: sr(Style.tinyMargin)
            color: Style.backgroundColor
            border {
                width: sr(Style.separatorHeight)
                color: Style.separatorColor
            }
        }
    }
}
