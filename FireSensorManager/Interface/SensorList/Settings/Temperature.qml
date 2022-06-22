import QtQuick
import QtQuick.Controls

import Custom.Sensors

Item {
    id: root

    property Sensor sensor
    property alias value: spinTemperature.value

    Label {
        id: lblTemperature

        height: root.height
        width: root.width * 0.45

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        text: qsTr("Temperature (°C):")
        color: "black"
    }

    SpinBox {
        id: spinTemperature

        width: root.width * 0.52

        anchors {
            left: lblTemperature.right
            leftMargin: root.width * 0.03
            top: root.top
            topMargin: (root.height - height) / 2
        }

        value: sensor.temperatureThreshold
        from: -20
        to: 100
        editable: true
    }
}
