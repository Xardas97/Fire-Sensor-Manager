import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    signal chosenFloorChanged(int floor)
    signal noAvailableFloors()

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

        onCurrentValueChanged: raiseFloorEvent()
    }

    Connections {
        target: service

        function onFloorAdded(floor) {
            var index = comboBoxFloors.indexOfValue(floor)
            comboBoxFloors.currentIndex = index
        }

        function onFloorRemoved(floor) {
            if (service.availableFloors.length > 0)
                comboBoxFloors.currentIndex = 0
        }
    }

    function raiseFloorEvent() {
        if (service.availableFloors.length == 0) {
            noAvailableFloors()
            return
        }

        chosenFloorChanged(comboBoxFloors.currentValue)
    }
}
