import QtQuick
import QtQuick.Controls

import Custom.Sensors

Item {
    id: root

    property Sensor sensor
    property alias value: spinPollution.value

    Label {
        id: lblPollution

        height: root.height
        width: root.width * 0.45

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        text: qsTr("Pollution (AQI):")
        color: "black"
    }

    SpinBox {
        id: spinPollution

        width: root.width * 0.52

        anchors {
            left: lblPollution.right
            leftMargin: root.width * 0.03
            top: root.top
            topMargin: (root.height - height) / 2
        }

        value: sensor.pollutionThreshold
        from: 0
        to: 500
        stepSize: 20
        editable: true
    }
}
