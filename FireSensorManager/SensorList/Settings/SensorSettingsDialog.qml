import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Dialog {
    id: root
    title: qsTr("Sensor Settings")

    property Sensor sensor

    standardButtons: Dialog.Save | Dialog.Cancel

    ColumnLayout {
        id: layout

        anchors.fill: parent

        SensorName {
            id: sensorName

            sensor: root.sensor

            Layout.fillWidth: true
            Layout.preferredHeight: layout.height * 0.15
            Layout.alignment: Qt.AlignCenter
        }

        Label {
            id: lblThresholds

            visible: temperature.visible | co2Concentration.visible | pollution.visible

            Layout.fillWidth: true
            Layout.preferredHeight: layout.height * 0.15

            verticalAlignment: Text.AlignBottom

            text: qsTr("Thresholds:")
            font.underline: true
        }

        Temperature {
            id: temperature

            visible: root.sensor.capabilities & 1

            sensor: root.sensor

            Layout.fillWidth: true
            Layout.preferredHeight: layout.height * 0.15
            Layout.alignment: Qt.AlignCenter
        }

        Co2Concentration {
            id: co2Concentration

            visible: root.sensor.capabilities & 4

            sensor: root.sensor

            Layout.fillWidth: true
            Layout.preferredHeight: layout.height * 0.15
            Layout.alignment: Qt.AlignCenter
        }

        Pollution {
            id: pollution

            visible: root.sensor.capabilities & 8

            sensor: root.sensor

            Layout.fillWidth: true
            Layout.preferredHeight: layout.height * 0.15
            Layout.alignment: Qt.AlignCenter
        }
    }

    onAccepted: {
        // TODO: Set this name onto the sensor
        sensor.name = sensorName.value
        sensor.temperatureThreshold = temperature.value
        sensor.co2ConcentrationThreshold = co2Concentration.value
        sensor.pollutionThreshold = pollution.value
    }

    onRejected: {
         sensorName.value = sensor.name
         temperature.value = sensor.temperatureThreshold
         co2Concentration.value = sensor.co2ConcentrationThreshold
         pollution.value = sensor.pollutionThreshold
    }
}
