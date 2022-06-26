import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

import "../Settings"
import "../FloorMaps"
import "UserControls"

Rectangle {
    id: root

    signal settingsFlowEntered
    signal settingsFlowLeft
    signal requestShowSensor(Sensor sensor)

    color: "grey"

    Button {
        id: btnSettingsMenu

        anchors.verticalCenter: root.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: 10

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Menu.png"
        background: Rectangle { opacity: 0 }

        onClicked: popupSettings.open()
    }

    SettingsPopup {
        id: popupSettings
        anchors.centerIn: Overlay.overlay

        onSettingsFlowEntered: root.settingsFlowEntered()
        onSettingsFlowLeft: root.settingsFlowLeft()
    }

    RowLayout {
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
        }

        LoggedInControls {
            Layout.alignment: Qt.AlignVCenter
            visible: service.isLoggedIn
        }

        LoggedOutControls {
            Layout.alignment: Qt.AlignVCenter
            visible: !service.isLoggedIn
        }
    }
}
