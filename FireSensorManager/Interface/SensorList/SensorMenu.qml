import QtQuick
import QtQuick.Controls

import Custom.Sensors

Menu {
    id: root

    property Sensor sensor
    signal settingsRequested()

    MenuItem {
        text: qsTr("Try to Reactivate")

        visible: !sensor.isActive || sensor.isReplaced
        height: visible? implicitHeight : 0

        enabled: !sensor.isReplaced
        onTriggered: service.reactivateSensor(sensor)
    }
    MenuItem {
        text: qsTr("Settings")
        enabled: service.users.hasModPermissions
        onTriggered: settingsRequested()
    }
    MenuItem {
        text: qsTr("Remove from Map")

        visible: sensor.isPlaced
        height: visible? implicitHeight : 0

        enabled: service.users.isLoggedIn
        onTriggered: service.removeFromMap(sensor)
    }
    MenuItem {
        text: qsTr("Remove")
        enabled: service.users.hasModPermissions
        onTriggered: service.removeSensor(sensor)
    }
}
