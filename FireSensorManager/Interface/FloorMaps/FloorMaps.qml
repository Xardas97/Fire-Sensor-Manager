import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

import "FloorPartView"

Item {
    id: root

    property Sensor selectedSensor
    signal selectedSensorPlaced()

    function showSensor(sensor) { floorPartView.showSensor(sensor) }

    FloorPartView {
        id: floorPartView

        anchors.fill: parent

        selectedSensor: root.selectedSensor
        onSelectedSensorPlaced: root.selectedSensorPlaced()
    }
}
