import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

RowLayout {
    id: root

    signal requestShowSensor(Sensor sensor)

    Button {
        id: btnAlarmStatus

        visible: service.warningTracker.alarmedSensors.length > 0

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Alarm.png"
        background: Rectangle { opacity: 0 }

        ToolTip.delay: 500
        ToolTip.visible: hovered
        ToolTip.text: qsTr("A sensor is reporting alarming signal values")

        onClicked: root.requestShowSensor(getNextSensorToShow())

        property int nextSensorToShowIndex: 0
        function getNextSensorToShow() {
            var alarmedSensors = service.warningTracker.alarmedSensors
            if (nextSensorToShowIndex >= alarmedSensors.length)
                nextSensorToShowIndex = 0

            return alarmedSensors[nextSensorToShowIndex++]
        }
    }


    Button {
        id: btnErrorStatus

        visible: service.warningTracker.warningSensors.length > 0

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Error.png"
        background: Rectangle { opacity: 0 }

        ToolTip.delay: 500
        ToolTip.visible: hovered
        ToolTip.text: qsTr("A sensor requires attention")

        onClicked: root.requestShowSensor(getNextSensorToShow())

        property int nextSensorToShowIndex: 0
        function getNextSensorToShow() {
            var warningSensors = service.warningTracker.warningSensors
            if (nextSensorToShowIndex >= warningSensors.length)
                nextSensorToShowIndex = 0

            return warningSensors[nextSensorToShowIndex++]
        }
    }
}
