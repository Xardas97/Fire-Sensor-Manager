import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    property int currentFloorView

    RowLayout {
        id: layoutFloors
        visible: currentFloorView === FloorMaps.FloorView.Floor || currentFloorView === FloorMaps.FloorView.FloorPart

        Label {
            id: lblFloors

            text: qsTr("Floor:")
            font.pointSize: 13
            color: "white"
        }

        ComboBox {
            id: comboBoxFloors
            model: service.availableFloors

            font.pointSize: 13

            onCurrentValueChanged: {
                comboBoxFloorParts.currentIndex = 0
                service.selectedFloor = currentValue !== undefined ? currentValue : null
            }
        }
    }

    RowLayout {
        id: layoutFloorParts
        visible: currentFloorView === FloorMaps.FloorView.FloorPart

        Label {
            id: lblFloorParts

            text: qsTr("Floor part:")
            font.pointSize: 13
            color: "white"
        }

        ComboBox {
            id: comboBoxFloorParts
            model: service.availableFloorParts
            font.pointSize: 13

            onCurrentValueChanged: {
                service.selectedFloorPart = currentValue !== undefined ? currentValue : null
            }
        }
    }

    Connections {
        target: service

        function onFloorAdded(floor) {
            comboBoxFloors.currentIndex = comboBoxFloors.indexOfValue(floor)
        }

        function onFloorRemoved(floor) {
            if (service.availableFloors.length > 0)
                comboBoxFloors.currentIndex = 0
        }

        function onFloorPartAdded(floor) {
            comboBoxFloors.currentIndex = comboBoxFloors.indexOfValue(floor)
            comboBoxFloorParts.currentIndex = service.availableFloorParts.length - 1
        }

        function onFloorPartRemoved(floor) { /* ... */ }

        function onSelectedFloorChanged() {
            comboBoxFloors.currentIndex = comboBoxFloors.indexOfValue(service.selectedFloor)
        }

        function onSelectedFloorPartChanged() {
            comboBoxFloorParts.currentIndex = service.selectedFloorPart
        }
    }
}
