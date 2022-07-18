import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int fontSize

    Label {
        id: lblTemperature

        height: root.height
        width: root.width * 0.45

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("Temperature (°C):")
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

        font.pixelSize: fontSize

        value: sensor.sensorState.temperature
        from: -20
        to: 100
        editable: true

        onValueModified: sensor.sensorState.temperature = value
    }
}
