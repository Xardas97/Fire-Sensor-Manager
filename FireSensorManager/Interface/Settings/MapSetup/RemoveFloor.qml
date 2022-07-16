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

        Item {
            Layout.fillWidth: true
            implicitWidth: comboBoxFloors.implicitWidth
            implicitHeight: comboBoxFloors.implicitHeight

            ComboBox {
                id: comboBoxFloors
                anchors.centerIn: parent
                model: service.availableFloors
            }
        }

        Button {
            text: qsTr("Remove")
            enabled: service.availableFloors.length > 0
            onClicked: service.removeFloor(comboBoxFloors.currentValue)
        }
    }
}
