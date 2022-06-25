import QtQuick
import QtQuick.Controls

import Custom.Sensors

import "../Settings"
import "../FloorMaps"

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

    Button {
        id: btnAlarm

        anchors.verticalCenter: root.verticalCenter
        anchors.right: floorMapControls.left
        anchors.rightMargin: 10

        visible: service.alarmedPlacedSensors.length > 0

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Alert.png"
        background: Rectangle { opacity: 0 }

        onClicked: root.requestShowSensor(getNextSensorToShow())

        property int nextSensorToShowIndex: 0
        function getNextSensorToShow() {
            var alarmedPlacedSensors = service.alarmedPlacedSensors
            if (nextSensorToShowIndex >= alarmedPlacedSensors.length)
                nextSensorToShowIndex = 0

            return alarmedPlacedSensors[nextSensorToShowIndex++]
        }
    }

    FloorMapControls {
        id: floorMapControls

        anchors.verticalCenter: root.verticalCenter
        anchors.right: root.right
        anchors.rightMargin: 10
    }
}
