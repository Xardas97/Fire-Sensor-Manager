import QtQuick
import QtQuick.Controls

Row {
    id: root

    property int fontSize

    Label {
        id: lblSmokeDetected

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("Smoke detected:")
        color: "black"
    }

    Item {
        height: root.height
        width: root.width * 0.03
    }

    CheckBox {
        id: chboxSmokeDetected

        height: root.height
        width: root.width * 0.62

        checked: server.sensorState.smokeDetected
        onCheckStateChanged: server.sensorState.smokeDetected = checked
    }
}
