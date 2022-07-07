import QtQuick
import QtQuick.Controls

import Custom.Sensors

Row {
    id: root

    property Sensor sensor
    property alias value: spinPollution.value

    spacing: 20

    Label {
        id: lblPollution

        width: (root.width - root.spacing) * 0.45

        anchors.verticalCenter: root.verticalCenter
        horizontalAlignment: "AlignRight"

        text: qsTr("Pollution (AQI):")
    }

    SpinBox {
        id: spinPollution

        width: (root.width - root.spacing) * 0.55

        anchors {
            topMargin: (root.height - height) / 2
            verticalCenter: root.verticalCenter
        }

        value: sensor.pollutionThreshold
        from: 0
        to: 500
        stepSize: 20
        editable: true
    }
}
