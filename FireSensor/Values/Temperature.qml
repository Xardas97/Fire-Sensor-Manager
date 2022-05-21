import QtQuick
import QtQuick.Controls

Row {
    id: root

    property int fontSize

    Label {
        id: lblTemperature

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("Temperature (°C):")
        color: "black"
    }

    Item {
        height: root.height
        width: root.width * 0.03
    }

    SpinBox {
        id: spinTemperature

        height: root.height
        width: root.width * 0.62

        font.pixelSize: fontSize

        value: server.sensorState.temperature
        from: -20
        to: 100
        editable: true

        onValueModified: server.sensorState.temperature = value
    }
}
