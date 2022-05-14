import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "Controls"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("FireSensorManager")

    ColumnLayout {
        anchors.fill: parent

        Label {
            id: txt

            Layout.alignment: Qt.AlignCenter

            text: "Just a random text"
            color: "purple"
        }

        Button {
            id: btnSensorRequest

            Layout.alignment: Qt.AlignCenter

            text: "Send request to sensor"

            onClicked: txt.text = "Server last returned: " + service.getNextNumber()
        }

        Button {
            id: btnDiscoverSensors

            Layout.alignment: Qt.AlignCenter

            text: "Discover sensors"

            onClicked: service.discoverSensors()
        }
    }
}
