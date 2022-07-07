import QtQuick
import QtQuick.Controls

import Custom.Sensors

Row {
    id: root

    property Sensor sensor
    property alias value: spinCo2Concentration.value

    spacing: 20

    Label {
        id: lblCo2Concentration

        width: (root.width - root.spacing) * 0.45

        anchors.verticalCenter: root.verticalCenter
        horizontalAlignment: "AlignRight"

        text: qsTr("CO2 Concentration (ppm):")
    }

    SpinBox {
        id: spinCo2Concentration

        width: (root.width - root.spacing) * 0.55

        anchors {
            topMargin: (root.height - height) / 2
            verticalCenter: root.verticalCenter
        }

        value: sensor.co2ConcentrationThreshold
        from: 0
        to: 100000
        stepSize: 200
        editable: true
    }
}
