import QtQuick
import QtQuick.Controls

import Custom.Sensors

Item {
    id: root

    property Sensor sensor
    property alias value: spinCo2Concentration.value

    Label {
        id: lblCo2Concentration

        height: root.height
        width: root.width * 0.45

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        text: qsTr("CO2 Concentration (ppm):")
        color: "black"
    }

    SpinBox {
        id: spinCo2Concentration

        width: root.width * 0.52

        anchors {
            left: lblCo2Concentration.right
            leftMargin: root.width * 0.03
            top: root.top
            topMargin: (root.height - height) / 2
        }

        value: sensor.co2ConcentrationThreshold
        from: 0
        to: 100000
        stepSize: 200
        editable: true
    }
}
