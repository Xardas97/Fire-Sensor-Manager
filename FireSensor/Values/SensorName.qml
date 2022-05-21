import QtQuick
import QtQuick.Controls

Row {
    id: root

    property int fontSize

    Label {
        id: lblName

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("Sensor Name:")
        color: "black"
    }

    Item {
        height: root.height
        width: root.width * 0.03
    }

    TextArea {
        id: txtName

        height: root.height
        width: root.width * 0.62

        horizontalAlignment: "AlignLeft"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: sensor.sensorState.name
        onEditingFinished: sensor.sensorState.name = text
    }
}
