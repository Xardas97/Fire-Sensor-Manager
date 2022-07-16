import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    property int currentFloorView

    RowLayout {
        id: layoutFloors
        height: root.height
        visible: currentFloorView === FloorMaps.FloorView.Floor || currentFloorView === FloorMaps.FloorView.FloorPart

        Label {
            id: lblFloors

            verticalAlignment: Text.AlignVCenter
            Layout.preferredHeight: layoutFloors.height

            text: qsTr("Floor:")
            font.pointSize: 13
        }

        ComboBox {
            id: comboBoxFloors
            model: service.availableFloors

            Layout.preferredHeight: layoutFloors.height

            font.pointSize: 13

            onCurrentValueChanged: {
                comboBoxFloorParts.currentIndex = 0
                service.selectedFloor = currentValue !== undefined ? currentValue : null
            }
        }
    }

    Item {
        Layout.fillHeight: true
        Layout.preferredWidth: spacing * 2
    }

    RowLayout {
        id: layoutFloorParts
        height: root.height
        visible: currentFloorView === FloorMaps.FloorView.FloorPart

        Label {
            id: lblFloorParts

            verticalAlignment: Text.AlignVCenter
            Layout.preferredHeight: layoutFloorParts.height

            text: qsTr("Floor part:")
            font.pointSize: 13
        }

        ComboBox {
            id: comboBoxFloorParts
            model: service.availableFloorParts
            font.pointSize: 13

            Layout.preferredHeight: layoutFloorParts.height

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
