import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Rectangle {
    id: root

    border {
        color: "black"
        width: 1
    }

    property Sensor selectedSensor: null

    signal requestShowSensor(Sensor sensor)

    ColumnLayout {
        id: column

        width: parent.width - 2
        height: parent.height - 2
        anchors.centerIn: parent

        CheckBox {
            id: chboxPlacedFilter

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Placed Sensors");

            checked: !service.knownSensorsModel.placedFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.placedFilterEnabled = !checked
        }

        CheckBox {
            id: chboxUnplacedFilter

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Unplaced Sensors");

            checked: !service.knownSensorsModel.unplacedFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.unplacedFilterEnabled = !checked
        }

        CheckBox {
            id: chboxinActiveFilter

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Inactive Sensors");

            checked: !service.knownSensorsModel.inactiveFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.inactiveFilterEnabled = !checked
        }

        ScrollView {
            Layout.preferredWidth: column.width
            Layout.fillHeight: true

            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                width: parent.width
                clip: true

                boundsMovement: Flickable.StopAtBounds

                model: service.knownSensorsModel
                delegate: SensorView {
                    sensor:  model.data
                    width: column.width
                    height: column.height / 10

                    isSelected: sensor == selectedSensor
                    onSelected: selectedSensor = sensor
                    onDeselected: selectedSensor = null

                    onRequestShowSensor: root.requestShowSensor(sensor)
                }
            }
        }

        Item {
            // padding item
            Layout.preferredHeight: column.height * 0.02
        }

        Button {
            id: btnDiscoverSensors
            Layout.alignment: Qt.AlignCenter

            text: qsTr("Discover sensors")
            onClicked: service.discoverSensors()
        }

        Item {
            // padding item
            Layout.preferredHeight: column.height * 0.02
        }
    }
}
