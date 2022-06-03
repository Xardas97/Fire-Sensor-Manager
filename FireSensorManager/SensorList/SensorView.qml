import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Custom.Sensors

Rectangle {
    id: root

    property Sensor sensor

    height: columnSensor.height * 1.2

    border {
        color: "black"
        width: 1
    }

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

        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        color: "grey"
        opacity: sensor != null ? (sensor.isActive ? 0 : 0.25) : 0
    }
}

