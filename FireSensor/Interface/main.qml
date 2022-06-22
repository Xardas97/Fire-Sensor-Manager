import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Values"

Window {
    id: root
    title: qsTr("FireSensor")

    minimumWidth: 400
    minimumHeight: 300
    width: minimumWidth * 1.3
    height: minimumHeight * 1.3

    visible: true

    property int fontSize: dummyText.font.pixelSize * Math.min(height / minimumHeight, width / minimumWidth)
    Text { id: dummyText }

    SensorSetup {
        id: sensorSetup

        fontSize: root.fontSize

        width: parent.width * 0.9
        height: parent.height * 0.5
        anchors.centerIn: parent

        onStartRequested: {
            sensorSetup.visible = false;

            var success = sensor.startSensor(enabledCapabilities)
            if (success)
                loaderMainPage.source = "RunningSensor.qml"
            else
                lblFailedToStart.visible = true
        }

        onLoadRequested: function(url) {
            sensorSetup.visible = false;

            var success = sensor.loadSensor(url)
            if (success)
                loaderMainPage.source = "RunningSensor.qml"
            else
                lblFailedToStart.visible = true
        }
    }

    Loader {
        id: loaderMainPage

        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.centerIn: parent

        onLoaded: {
            item.onCloseRequested.connect(onCloseRequested)
            item.fontSize = Qt.binding(function() { return root.fontSize})
        }

        function onCloseRequested() {
            loaderMainPage.source = ""
            sensorSetup.visible = true
            sensor.stopSensor()
        }
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
