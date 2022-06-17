import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "TopLine"
import "FloorMaps"
import "SensorList"

Window {
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

            popupSettings {
                onOpened: columnWindow.opacity = 0.3
                onClosed: columnWindow.opacity = 1
            }

            onChosenFloorChanged: function(floor) {
                console.log("Chosen floor changed to: " + floor)
                floorMaps.chosenFloor = floor
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
            }

            FloorMaps {
                id: floorMaps

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }

}
