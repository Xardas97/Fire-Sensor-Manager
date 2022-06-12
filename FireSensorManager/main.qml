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
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width * 0.15
        }
    }
}
