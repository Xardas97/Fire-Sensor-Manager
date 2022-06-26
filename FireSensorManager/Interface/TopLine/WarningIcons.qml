import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

RowLayout {
    id: root

    signal requestShowSensor(Sensor sensor)

    Button {
        id: btnAlarmStatus

        visible: service.alarmedPlacedSensors.length > 0

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Alarm.png"
        background: Rectangle { opacity: 0 }

        onClicked: root.requestShowSensor(getNextSensorToShow())

        property int nextSensorToShowIndex: 0
        function getNextSensorToShow() {
            var alarmedPlacedSensors = service.alarmedPlacedSensors
            if (nextSensorToShowIndex >= alarmedPlacedSensors.length)
                nextSensorToShowIndex = 0

            return alarmedPlacedSensors[nextSensorToShowIndex++]
        }
    }


    Button {
        id: btnErrorStatus

        visible: service.placedSensorsWithStatus.length > 0

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Error.png"
        background: Rectangle { opacity: 0 }

        onClicked: root.requestShowSensor(getNextSensorToShow())

        property int nextSensorToShowIndex: 0
        function getNextSensorToShow() {
            var placedSensorsWithStatus = service.placedSensorsWithStatus
            if (nextSensorToShowIndex >= placedSensorsWithStatus.length)
                nextSensorToShowIndex = 0

            return placedSensorsWithStatus[nextSensorToShowIndex++]
        }
    }
}
