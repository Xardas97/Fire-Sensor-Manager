import QtQuick
import QtQuick.Controls

Row {
    id: root

    property int fontSize

    Label {
        id: lblCo2Concentration

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("CO2 Concentration (ppm):")
        color: "black"
    }

    Item {
        height: root.height
        width: root.width * 0.03
    }

    SpinBox {
        id: spinCo2Concentration

        height: root.height
        width: root.width * 0.62

        font.pixelSize: fontSize

        value: server.sensorState.co2Concentration
        from: 0
        to: 100000
        stepSize: 200
        editable: true

        onValueModified: server.sensorState.co2Concentration = value
    }
}
