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

    border {
        color: "black"
        width: 1
    }

    Label {
        id: txtName

        width: root.width - btnSettings.width - 0.15 * root.width
        height: root.height

        anchors {
            left: root.left
            leftMargin: root.width * 0.05
            rightMargin: root.width * 0.05
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        elide: Text.ElideRight
        font.pointSize: 15

        text: sensor.name
        color: sensor.alarmOn ? "red" : (root.errorStatus || root.dirtyStatus | root.maintenanceRequiredStatus) ? "purple" : "black"
    }

    Button {
        id: btnSettings

        anchors {
            right: root.right
            rightMargin: root.width * 0.05
            verticalCenter: root.verticalCenter
        }

        icon.source: "qrc:/Icons/SensorSettings.png"

        background: Rectangle { opacity: 0 }
    }

    SensorToolTip {
        id: sensorToolTip
        anchors.fill: parent

        sensor: root.sensor
    }

    Rectangle {
        id : rectInactive

        width: root.width
        height: root.height
        anchors.centerIn: root

        color: "grey"
        opacity: sensor.isActive || sensor.isReplaced ? 0 : 0.25
    }

    Rectangle {
        id : rectReplaced

        width: root.width
        height: root.height
        anchors.centerIn: root

        color: "red"
        opacity: sensor.isReplaced ? 0.25 : 0
    }
}

