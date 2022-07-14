import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

import "../Controls"
import "../Settings"
import "../FloorMaps"
import "UserControls"

import "../common.js" as Common

Item {
    id: root

    property int currentFloorView

    signal requestShowSensor(Sensor sensor)

    height: Math.max(btnSettingsMenu.height, rowLayout.height)

    Rectangle {
        id: rectBackground
        anchors.fill: parent

        opacity: 0.25
        color: Common.isLight(Material.background)? "black" : "white"
    }

    IconButton {
        id: btnSettingsMenu

        anchors.verticalCenter: root.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: 10

        icon.source: "qrc:/Resources/Icons/Menu.png"

        onClicked: popupSettings.open()
    }

    SettingsPopup {
        id: popupSettings
        modal: true
        anchors.centerIn: Overlay.overlay
    }

    RowLayout {
        id: rowLayout

        anchors.verticalCenter: root.verticalCenter
        anchors.right: root.right
        anchors.rightMargin: 10

        WarningIcons {
            id: warningIcons

            Layout.alignment: Qt.AlignVCenter

            onRequestShowSensor: function(sensor) {
                root.requestShowSensor(sensor)
            }
        }

        FloorMapControls {
            id: floorMapControls
            Layout.alignment: Qt.AlignVCenter
            currentFloorView: root.currentFloorView
        }

        Item {
            id: padding
            Layout.fillHeight: true
            Layout.preferredWidth: 20
        }

        LoggedInControls {
            Layout.alignment: Qt.AlignVCenter
            visible: service.users.isLoggedIn
        }

        LoggedOutControls {
            Layout.alignment: Qt.AlignVCenter
            visible: !service.users.isLoggedIn
        }
    }
}
