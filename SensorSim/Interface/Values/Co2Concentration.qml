import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int fontSize

    Label {
        id: lblCo2Concentration

        height: root.height
        width: root.width * 0.45

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("CO2 Concentration (ppm):")
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

        font.pixelSize: fontSize

        value: sensor.sensorState.co2Concentration
        from: 0
        to: 100000
        stepSize: 200
        editable: true

        onValueModified: sensor.sensorState.co2Concentration = value
    }
}
