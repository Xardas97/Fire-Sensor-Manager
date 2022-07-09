import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "TopLine"
import "FloorMaps"
import "SensorList"

ApplicationWindow {
    id: root

    title: qsTr("FireSensorManager")
    visible: true

    width: 1360
    height: 765
    minimumWidth: 1056
    minimumHeight: 594

    ColumnLayout {
        id: columnWindow
        anchors.fill: parent

        TopLine {
            Layout.fillWidth: true
            Layout.preferredHeight: root.height / 20

            currentFloorView: floorMaps.currentFloorView
            onRequestShowSensor: function (sensor) {
                floorMaps.showSensor(sensor)
            }
        }

        RowLayout {
            id: rowWindow

            Layout.fillWidth: true
            Layout.fillHeight: true

            SensorList {
                id: listKnownSensors
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.15

                onRequestShowSensor: function (sensor) {
                    floorMaps.showSensor(sensor)
                }
            }

            FloorMaps {
                id: floorMaps

                Layout.fillHeight: true
                Layout.fillWidth: true

                selectedSensor: listKnownSensors.selectedSensor
                onSelectedSensorPlaced: listKnownSensors.selectedSensor = null
            }
        }
    }

}
