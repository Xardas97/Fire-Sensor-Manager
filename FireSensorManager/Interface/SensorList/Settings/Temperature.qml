import QtQuick
import QtQuick.Controls

import Custom.Sensors

Row {
    id: root

    property Sensor sensor
    property alias value: spinTemperature.value

    spacing: 20

    Label {
        id: lblTemperature

        width: (root.width - root.spacing) * 0.45

        anchors.verticalCenter: root.verticalCenter
        horizontalAlignment: "AlignRight"

        text: qsTr("Temperature (°C):")
    }

    SpinBox {
        id: spinTemperature

        width: (root.width - root.spacing) * 0.55

        anchors {
            topMargin: (root.height - height) / 2
            verticalCenter: root.verticalCenter
        }

        value: sensor.temperatureThreshold
        from: -20
        to: 100
        editable: true
    }
}
