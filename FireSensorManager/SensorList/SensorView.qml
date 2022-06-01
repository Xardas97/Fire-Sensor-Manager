import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Custom.Sensors

Item {
    id: root

    property Sensor sensor

    ColumnLayout {
        id: columnSensor

        width: parent.width
        anchors.centerIn: parent

        Label {
            id: txtName

            Layout.alignment: Qt.AlignCenter

            text: sensor != null ? sensor.name : "Null"
            color: "black"
        }

        Label {
            id: txtTemperature

            Layout.alignment: Qt.AlignCenter

            text: sensor != null ? sensor.temperature : "Null"
            color: "black"
        }
    }

    Rectangle {
        id : rectBackground

        width: columnSensor.width
        height: columnSensor.height
        anchors.centerIn: parent

        color: "grey"
        opacity: sensor != null ? (sensor.isActive ? 0 : 0.25) : 0
    }
}

