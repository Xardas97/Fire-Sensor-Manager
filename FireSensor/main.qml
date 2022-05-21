import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Values"

Window {
    id: root
    title: qsTr("FireSensor")

    minimumWidth: 400
    minimumHeight: 250
    width: minimumWidth * 2
    height: minimumHeight * 2

    visible: true

    property int fontSize: dummyText.font.pixelSize * Math.min(height / minimumHeight, width / minimumWidth)
    Text { id: dummyText }

    SensorSetup {
        id: sensorSetup

        fontSize: root.fontSize

        width: parent.width * 0.9
        height: parent.height * 0.5
        anchors.centerIn: parent

        btnStartSensor.onClicked: {
            sensorSetup.visible = false;

            var success = sensor.startSensor(enabledCapabilities)
            if (success)
                loaderMainPage.source = "Values/DynamicSensorValues.qml"
            else
                lblFailedToStart.visible = true

        }
    }

    Loader {
        id: loaderMainPage

        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.centerIn: parent

        onLoaded: item.fontSize = Qt.binding(function() { return root.fontSize})
    }

    Label {
        id: lblFailedToStart

        visible: false

        anchors.fill: parent
        verticalAlignment: "AlignVCenter"
        horizontalAlignment: "AlignHCenter"

        font.pixelSize: root.fontSize

        text: "FAILED TO START, NO PORT AVAILABLE"
        color: "red"
    }
}
