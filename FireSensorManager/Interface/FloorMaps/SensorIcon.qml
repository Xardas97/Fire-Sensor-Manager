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

    Connections {
        target: service
        function onSelectedFloorChanged() { root.destroy() }
        function onSelectedFloorPartChanged() { root.destroy() }
    }

    Connections {
        target: sensor
        function onRemovedFromMap() { root.destroy() }
    }

    SensorToolTip {
        id: sensorToolTip
        anchors.fill: root
        sensor: root.sensor
    }

    onSensorChanged: {
        if (sensor == null)
            return;

        sensor.x = Qt.binding(function() { return x })
        sensor.y = Qt.binding(function() { return y })
    }
}
