import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Dialog {
    id: root
    title: qsTr("Sensor Settings")

    property Sensor sensor
    property int desiredX
    property int desiredY

    width: 400

    standardButtons: Dialog.Save | Dialog.Cancel
    Component.onCompleted:standardButton(Dialog.Save).enabled = Qt.binding(isSensorNameValueValid)
    function isSensorNameValueValid() { return sensorName.value.trim().length > 0 }

    ColumnLayout {
        id: layout

        anchors.fill: parent

        SensorName {
            id: sensorName

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
        }

        Label {
            id: lblFailedUpdate

            visible: false

            Layout.fillWidth: true

            text: qsTr("Failed to update the sensor name")
            color: "red"
        }

        Item {
            id: itemLabelPadding

            visible: !lblFailedUpdate.visible

            Layout.fillWidth: true
            Layout.preferredHeight: lblFailedUpdate.implicitHeight
        }

        Label {
            id: lblThresholds

            visible: temperature.visible | co2Concentration.visible | pollution.visible

            Layout.fillWidth: true

            verticalAlignment: Text.AlignBottom

            text: qsTr("Thresholds:")
            font.underline: true
        }

        Temperature {
            id: temperature

            visible: root.sensor.capabilities & 1

            sensor: root.sensor

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }

        Co2Concentration {
            id: co2Concentration

            visible: root.sensor.capabilities & 4

            sensor: root.sensor

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }

        Pollution {
            id: pollution

            visible: root.sensor.capabilities & 8

            sensor: root.sensor

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }

        Item {
            id: itemTemperaturePadding

            visible: !temperature.visible

            Layout.fillWidth: true
            Layout.preferredHeight: temperature.implicitHeight
        }

        Item {
            id: itemCo2ConcentrationPadding

            visible: !co2Concentration.visible

            Layout.fillWidth: true
            Layout.preferredHeight: co2Concentration.implicitHeight
        }

        Item {
            id: itemPollutionPadding

            visible: !pollution.visible

            Layout.fillWidth: true
            Layout.preferredHeight: pollution.implicitHeight
        }
    }

    // Workaround to make sure sensor name text field is not continuously updated
    onSensorChanged: {
        sensorName.currentName = sensor.name
        sensor.onNameChanged.connect(updateSensorName)
    }
    Component.onDestruction: sensor.onNameChanged.disconnect(updateSensorName)
    function updateSensorName() {
        if (sensor.name != sensorName.currentName)
            sensorName.currentName = sensor.name
    }

    property bool updateFailed: false

    onAccepted: {
        if (sensor.name != sensorName.value) {
            var updated = service.updateName(sensor, sensorName.value)
            if (!updated) {
                updateFailed = true
                return
            }
        }

        sensor.temperatureThreshold = temperature.value
        sensor.co2ConcentrationThreshold = co2Concentration.value
        sensor.pollutionThreshold = pollution.value
    }

    onRejected: {
         sensorName.value = sensor.name
         temperature.value = sensor.temperatureThreshold
         co2Concentration.value = sensor.co2ConcentrationThreshold
         pollution.value = sensor.pollutionThreshold
    }

    onClosed: {
        lblFailedUpdate.visible = updateFailed
        if (updateFailed) {
            root.open()
            updateFailed = false;
        }
    }

    onAboutToShow: {
        x = getDialogX(desiredX)
        y = getDialogY(desiredY)
    }

    function getDialogX(desiredX) {
        var maxDialogX = parent.Window.window.x + parent.Window.window.width - root.width - mapToGlobal(0, 0).x - 20 * parent.scale
        return Math.min(desiredX, maxDialogX);
    }

    function getDialogY(desiredY) {
        var maxDialogY = parent.Window.window.y + parent.Window.window.height - root.height - mapToGlobal(0, 0).y - 20 * parent.scale
        return Math.min(desiredY, maxDialogY);
    }
}
