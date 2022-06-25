import QtQuick

import Custom.Sensors

import "../SensorList"

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
