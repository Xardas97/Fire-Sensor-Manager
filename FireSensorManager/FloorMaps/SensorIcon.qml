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
        source: "qrc:/Icons/Sensor.png"
    }

    Connections {
        target: service
        function onSelectedFloorChanged() { root.destroy() }
        function onSelectedFloorPartChanged() { root.destroy() }
    }

    SensorToolTip {
        id: sensorToolTip
        anchors.fill: root
        sensor: root.sensor
    }
}
