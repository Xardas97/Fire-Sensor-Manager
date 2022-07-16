import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: root

    property int fontSize

    SensorName {
        id: sensorName

        fontSize: root.fontSize

        Layout.fillWidth: true
        Layout.preferredHeight: root.height * 0.15
        Layout.alignment: Qt.AlignCenter
    }

    SensorStatuses {
        id: sensorStatuses

        fontSize: root.fontSize

        Layout.fillWidth: true
        Layout.preferredHeight: root.height * 0.15
        Layout.alignment: Qt.AlignCenter
    }

    SmokeDetection {
        id: smokeDetection

        visible: sensor.sensorState.capabilities & 2

        fontSize: root.fontSize

        Layout.fillWidth: true
        Layout.preferredHeight: root.height * 0.15
        Layout.alignment: Qt.AlignCenter
    }

    Temperature {
        id: temperature

        visible: sensor.sensorState.capabilities & 1

        fontSize: root.fontSize

        Layout.fillWidth: true
        Layout.preferredHeight: root.height * 0.15
        Layout.alignment: Qt.AlignCenter
    }

    Co2Concentration {
        id: co2Concentration

        visible: sensor.sensorState.capabilities & 4

        fontSize: root.fontSize

        Layout.fillWidth: true
        Layout.preferredHeight: root.height * 0.15
        Layout.alignment: Qt.AlignCenter
    }

    Pollution {
        id: pollution

        visible: sensor.sensorState.capabilities & 8

        fontSize: root.fontSize

        Layout.fillWidth: true
        Layout.preferredHeight: root.height * 0.15
        Layout.alignment: Qt.AlignCenter
    }
}
