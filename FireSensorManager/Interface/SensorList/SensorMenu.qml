import QtQuick
import QtQuick.Controls

import Custom.Sensors

Menu {
    id: root

    property Sensor sensor
    signal settingsRequested()

    MenuItem {
        id: menuItemReactivate
        text: qsTr("Try to Reactivate")

        visible: !sensor.isActive || sensor.isReplaced
        enabled: !sensor.isReplaced
        height: visible? implicitHeight : 0

        onTriggered: service.reactivateSensor(sensor)
    }
    MenuItem {
        text: qsTr("Settings")
        onTriggered: settingsRequested()
    }
    MenuItem {
        text: qsTr("Remove from Map")
        visible: sensor.isPlaced
        height: visible? implicitHeight : 0
        onTriggered: service.removeFromMap(sensor)
    }
    MenuItem {
        text: qsTr("Remove")
        onTriggered: service.removeSensor(sensor)
    }
}
