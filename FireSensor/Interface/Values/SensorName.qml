import QtQuick
import QtQuick.Controls

Item {
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

    TextArea {
        id: txtName

        width: root.width * 0.62

        anchors {
            left: lblName.right
            leftMargin: root.width * 0.03
            top: root.top
            topMargin: (root.height - height) / 2
        }

        horizontalAlignment: "AlignLeft"

        font.pixelSize: fontSize

        text: sensor.sensorState.name
        onEditingFinished: sensor.sensorState.name = text
    }
}
