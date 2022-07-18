import QtQuick
import QtQuick.Controls

Row {
    id: root

    property int fontSize

    Label {
        id: lblSmokeDetected

        height: root.height
        width: root.width * 0.45

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("Smoke detected:")
    }

    Item {
        height: root.height
        width: root.width * 0.03
    }

    CheckBox {
        id: chboxSmokeDetected

        height: root.height

        checked: sensor.sensorState.smokeDetected
        onCheckStateChanged: sensor.sensorState.smokeDetected = checked
    }
}
