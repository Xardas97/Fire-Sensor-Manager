import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "SensorList"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("FireSensorManager")

    RowLayout {
        id: rowWindow

        anchors.fill: parent

        SensorView {
            id: sensorView
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width * 0.15
        }

        ColumnLayout {
            id: columnMain

            Layout.preferredHeight: parent.height
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            Button {
                id: btnSensorRequest

                Layout.alignment: Qt.AlignCenter

                text: "Set a sensor"

                onClicked: sensorView.sensor = service.getRandomSensor()
            }

            Button {
                id: btnDiscoverSensors

                Layout.alignment: Qt.AlignCenter

                text: "Discover sensors"

                onClicked: service.discoverSensors()
            }
        }
    }
}
