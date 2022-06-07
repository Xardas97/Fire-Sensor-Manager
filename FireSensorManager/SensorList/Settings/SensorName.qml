import QtQuick
import QtQuick.Controls

import Custom.Sensors

Item {
    id: root

    property Sensor sensor
    property alias value: txtName.text

    Label {
        id: lblName

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

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

        text: sensor.name
    }
}
