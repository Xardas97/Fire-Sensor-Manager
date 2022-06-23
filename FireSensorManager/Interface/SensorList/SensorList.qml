import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Rectangle {
    border {
        color: "black"
        width: 1
    }

    property Sensor selectedSensor: null

    ColumnLayout {
        id: root

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
            Layout.preferredWidth: root.width
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
                    width: root.width
                    height: root.height / 10

                    isSelected: sensor == selectedSensor
                    onSelected: selectedSensor = sensor
                    onDeselected: selectedSensor = null
                }
            }
        }

        Item {
            // padding item
            Layout.preferredHeight: root.height * 0.02
        }

        Button {
            id: btnDiscoverSensors
            Layout.alignment: Qt.AlignCenter

            text: qsTr("Discover sensors")
            onClicked: service.discoverSensors()
        }

        Item {
            // padding item
            Layout.preferredHeight: root.height * 0.02
        }
    }
}
