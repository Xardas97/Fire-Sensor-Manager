import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    signal chosenFloorChanged(int floor)

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

        onActivated: chosenFloorChanged(currentValue)
        Component.onCompleted: chosenFloorChanged(currentValue)
    }
}
