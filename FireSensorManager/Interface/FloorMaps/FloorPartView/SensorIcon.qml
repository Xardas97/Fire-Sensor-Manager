import QtQuick
import QtQuick.Controls

import Custom.Sensors

import "../../SensorList"
import "../../SensorList/Settings"

Item {
    id: root

    signal pressed()
    signal released()
    property bool draggingEnabled: service.users.isLoggedIn

    width: rowContent.width
    height: rowContent.height

    property Sensor sensor

    Column {
        id: rowContent

        Label {
            id: lblName
            width: image.width * 1.2

            color: sensor.alarmOn ? "red" : (sensor.status > 0) ? "purple" : "black"
            text: sensor.name
            elide: Text.ElideRight

            font.bold: true
            font.pointSize: 11

            horizontalAlignment: Qt.AlignHCenter
        }

        Image {
            id: image
            source: "qrc:/Resources/Icons/Sensor.png"
            anchors.horizontalCenter: lblName.horizontalCenter
        }
    }


    MouseArea {
        id: dragArea
        anchors.fill: root

        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: function(mouse) {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }

        drag {
            target: draggingEnabled? root : undefined
            axis: Drag.XAndYAxis

            minimumX: 0
            minimumY: 0
            maximumX: root.parent.width - root.width
            maximumY: root.parent.height - root.height
        }

        onPressed: root.pressed()
        onReleased: root.released()
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
