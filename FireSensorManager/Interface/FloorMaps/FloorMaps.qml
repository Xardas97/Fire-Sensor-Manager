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
    property int currentFloorView: FloorMaps.FloorView.Building

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

        onShowPreviousView: {
            if (service.selectedFloorSize === 1) {
                currentFloorView = FloorMaps.FloorView.Building
                return
            }

            currentFloorView = FloorMaps.FloorView.Floor
        }
    }

    FloorView {
        id: floorView

        visible: currentFloorView === FloorMaps.FloorView.Floor
        anchors.fill: parent

        onShowPreviousView: currentFloorView = FloorMaps.FloorView.Building
        onShowFloorPartView: currentFloorView = FloorMaps.FloorView.FloorPart
    }

    BuildingView {
        id: buildingView

        visible: currentFloorView === FloorMaps.FloorView.Building
        anchors.fill: parent

        onShowFloorView: {
            if (service.selectedFloorSize === 1) {
                service.selectedFloorPart = 0
                currentFloorView = FloorMaps.FloorView.FloorPart
                return
            }

            currentFloorView = FloorMaps.FloorView.Floor
        }
    }
}
