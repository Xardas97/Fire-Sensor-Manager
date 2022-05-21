import QtQuick
import QtQuick.Controls

Row {
    id: root

    property int fontSize

    Label {
        id: lblPollution

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("Pollution (AQI):")
        color: "black"
    }

    Item {
        height: root.height
        width: root.width * 0.03
    }

    SpinBox {
        id: spinPollution

        height: root.height
        width: root.width * 0.62

        font.pixelSize: fontSize

        value: server.sensorState.pollution
        from: 0
        to: 500
        stepSize: 20
        editable: true

        onValueModified: server.sensorState.pollution = value
    }
}
