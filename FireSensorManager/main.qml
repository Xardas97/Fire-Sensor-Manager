import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "SensorList"

Window {
    title: qsTr("FireSensorManager")
    visible: true

    width: 1360
    height: 765
    minimumWidth: 1056
    minimumHeight: 594

    RowLayout {
        id: rowWindow

        anchors.fill: parent

        SensorList {
            id: listKnownSensors
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.15
        }

        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            source: "image://MapImageProvider/0/0"
        }
    }
}
