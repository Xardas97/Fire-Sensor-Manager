import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Custom.Sensors

import "Settings"
import "../Controls"

import "../common.js" as Common

Rectangle {
    id: root

    property Sensor sensor

    property bool isSelected: false
    signal selected()
    signal deselected()
    signal requestShowSensor()

    property bool errorStatus: sensor.status & 1
    property bool dirtyStatus: sensor.status & 2
    property bool maintenanceRequiredStatus: sensor.status & 4

    color: "transparent"
    border {
        color: isSelected? "red" : "black"
        width: isSelected? 2 : 1
    }

    ListView.onRemove: if (isSelected) deselected()

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton

        pressAndHoldInterval: 500

        onClicked: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                if (isSelected) deselected()
                else if (sensor.isPlaced) requestShowSensor()
            }
            else if (mouse.button === Qt.RightButton) {
                contextMenu.popup()
            }
        }

        onPressAndHold: function(mouse) {
            if (mouse.button === Qt.LeftButton && !isSelected && service.users.hasModPermissions)
                selected()
        }
    }

    Label {
        id: txtName

        width: root.width - btnSettings.width - 0.2 * root.width
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
        // used to get default text color
        Label { id: lbl; width: 0; height: 0 }
        color: sensor.alarmOn ? "red" : (root.errorStatus || root.dirtyStatus | root.maintenanceRequiredStatus) ? "purple" : lbl.color
    }

    IconButton {
        id: btnSettings

        anchors {
            right: root.right
            rightMargin: root.width * 0.1
            verticalCenter: root.verticalCenter
        }

        icon.source: "qrc:/Resources/Icons/SensorSettings.png"

        enabled: service.users.hasModPermissions
        onClicked: sensorSettingsDialog.open()
    }

    SensorMenu {
        id: contextMenu

        sensor: root.sensor
        selectOptionVisible: true

        onSelectRequested: root.selected()
        onSettingsRequested: sensorSettingsDialog.open()
    }

    SensorToolTip {
        id: sensorToolTip
        anchors.fill: txtName
        sensor: root.sensor
    }

    SensorSettingsDialog {
        id: sensorSettingsDialog
        sensor: root.sensor
        desiredX: btnSettings.x + btnSettings.width / 2
        desiredY: btnSettings.y + btnSettings.height / 2
    }

    Rectangle {
        id : rectBackground
        anchors.fill: parent
        color: internal.getBackgroundFilter(root.Material.background)
        opacity: 0.25
    }

    QtObject {
        id: internal
        function getBackgroundFilter(backgroundColor) {
            var inactive = root.sensor.isReplaced || !root.sensor.isActive
            var lightBackground = Common.isLight(backgroundColor)

            if (lightBackground === inactive)
                return "grey"

            return "transparent"
        }
    }
}

