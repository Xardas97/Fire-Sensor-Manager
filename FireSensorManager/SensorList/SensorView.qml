import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Custom.Sensors

Rectangle {
    id: root

    property Sensor sensor

    property bool errorStatus: sensor.status & 1
    property bool dirtyStatus: sensor.status & 2
    property bool maintenanceRequiredStatus: sensor.status & 4

    height: columnSensor.height * 2

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

            text: sensor.name
            color: sensor.alarmOn ? "red" : (root.errorStatus || root.dirtyStatus | root.maintenanceRequiredStatus) ? "purple" : "black"
        }
    }

    SensorToolTip {
        id: sensorToolTip
        anchors.fill: parent

        sensor: root.sensor
    }

    Rectangle {
        id : rectInactive

        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        color: "grey"
        opacity: sensor.isActive || sensor.isReplaced ? 0 : 0.25
    }

    Rectangle {
        id : rectReplaced

        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        color: "red"
        opacity: sensor.isReplaced ? 0.25 : 0
    }
}

