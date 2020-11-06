import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import "../items" as Items
import "../components" as Components

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

Items.GPanel
{
    id: splashScreen

    ColumnLayout {
        anchors.fill: parent

        Items.SvgImage
        {
            id: logoImage
            anchors {
                left: parent.left
                right: parent.right
            }

            height: splashScreen.height * 0.35
            source: GStyle.logoMalebiCharcoalTraceUrl
        }

        Items.LayoutSpacer {
            Layout.maximumHeight: s(GStyle.middleSmallMargin)
        }

        RowLayout {
            Layout.fillWidth: true

            Layout.alignment: Qt.AlignHCenter
            spacing: s(GStyle.bigMargin * 2)

            Items.SvgImage
            {
                id: logoMilo

                height: s(GStyle.logoHeight)
                source: GStyle.logoMiloUrl
            }

            Items.SvgImage
            {
                id: logoGaia

                height: s(GStyle.logoHeight)
                source: GStyle.logoGaiaUrl
            }
        }
    }
}
