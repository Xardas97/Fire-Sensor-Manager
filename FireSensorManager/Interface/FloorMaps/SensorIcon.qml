import QtQuick
import QtQuick.Controls

import Custom.Sensors

import "../SensorList"
import "../SensorList/Settings"

Rectangle {
    id: root

    width: image.width
    height: image.height

    color: "transparent"

    property Sensor sensor

    Image {
        id: image
        source: "qrc:/Resources/Icons/Sensor.png"
    }

    MouseArea {
        id: dragArea
        anchors.fill: root

        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: function(mouse) {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }

        drag.target: root
        drag.axis: Drag.XAndYAxis

        drag.minimumX: 0
        drag.minimumY: 0
        drag.maximumX: root.parent.width - root.width
        drag.maximumY: root.parent.height - root.height
    }

    SensorToolTip {
        id: sensorToolTip
        anchors.fill: root
        sensor: root.sensor

        toolTipTurnedOff: dragArea.drag.active
    }

    SensorSettingsDialog {
        id: sensorSettingsDialog
        sensor: root.sensor
        desiredX: root.width / 2
        desiredY: root.height / 2
    }

    SensorMenu {
        id: contextMenu
        sensor: root.sensor
        onSettingsRequested: sensorSettingsDialog.open()
    }

    onSensorChanged: {
        if (sensor == null)
            return;

        sensor.x = Qt.binding(function() { return x })
        sensor.y = Qt.binding(function() { return y })
    }

    Connections {
        target: service
        function onSelectedFloorChanged() { root.destroy() }
        function onSelectedFloorPartChanged() { root.destroy() }
    }

    Connections {
        target: sensor
        function onRemovedFromMap() { root.destroy() }
    }
}
