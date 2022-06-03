import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "SensorList"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("FireSensorManager")

    RowLayout {
        id: rowWindow

        anchors.fill: parent

        SensorList {
            id: listKnownSensors
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width * 0.15
        }

        Button {
            id: btnDiscoverSensors

            Layout.alignment: Qt.AlignCenter

            text: "Discover sensors"

            onClicked: service.discoverSensors()
        }
    }
}
