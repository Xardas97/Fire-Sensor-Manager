import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int fontSize

    Label {
        id: lblPollution

        height: root.height
        width: root.width * 0.45

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

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

        font.pixelSize: fontSize

        value: sensor.sensorState.pollution
        from: 0
        to: 500
        stepSize: 20
        editable: true

        onValueModified: sensor.sensorState.pollution = value
    }
}
