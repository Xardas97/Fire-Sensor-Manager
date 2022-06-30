import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

import "FloorPartView"

Item {
    id: root

    property Sensor selectedSensor
    signal selectedSensorPlaced()

    enum FloorView { FloorPart, Floor, Building }
    property int currentFloorView: FloorMaps.FloorView.Floor

    function showSensor(sensor) {
        currentFloorView = FloorMaps.FloorView.FloorPart
        floorPartView.showSensor(sensor)
    }

    FloorPartView {
        id: floorPartView

        visible: currentFloorView === FloorMaps.FloorView.FloorPart
        anchors.fill: parent

        selectedSensor: root.selectedSensor
        onSelectedSensorPlaced: root.selectedSensorPlaced()
        onShowFloorView: currentFloorView = FloorMaps.FloorView.Floor
    }

    FloorView {
        id: floorView

        visible: currentFloorView === FloorMaps.FloorView.Floor
        anchors.fill: parent

        onShowFloorPartView: currentFloorView = FloorMaps.FloorView.FloorPart
    }
}
