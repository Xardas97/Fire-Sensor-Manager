import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: root

    Label {
        text: qsTr("Remove floor");
        font.underline: true
    }

    RowLayout {
        Label {
            text: qsTr("Floor:")
        }

        ComboBox {
            id: comboBoxFloors
            model: service.availableFloors
        }

        Button {
            text: qsTr("Remove")
            enabled: service.availableFloors.length > 0
            onClicked: service.removeFloor(comboBoxFloors.currentValue)
        }
    }
}
