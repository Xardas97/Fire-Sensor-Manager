import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Rectangle {
    border {
        color: "black"
        width: 1
    }

    property Sensor selectedSensor: null

    ColumnLayout {
        id: root

        width: parent.width - 2
        height: parent.height - 2
        anchors.centerIn: parent

        CheckBox {
            id: chboxReplaceFilter

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Replaced Sensors");

            checked: !service.knownSensorsModel.replaceFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.replaceFilterEnabled = !checked
        }

        CheckBox {
            id: chboxActiveFilter

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Inactive Sensors");

            checked: !service.knownSensorsModel.activeFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.activeFilterEnabled = !checked
        }

        ScrollView {
            Layout.preferredWidth: root.width
            Layout.fillHeight: true

            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                width: parent.width
                clip: true

                boundsMovement: Flickable.StopAtBounds

                model: service.knownSensorsModel
                delegate: SensorView {
                    sensor:  model.data
                    width: root.width
                    height: root.height / 10

                    isSelected: sensor == selectedSensor
                    onSelected: selectedSensor = sensor
                    onDeselected: selectedSensor = null
                }
            }
        }

        Item {
            // padding item
            Layout.preferredHeight: root.height * 0.02
        }

        Button {
            id: btnDiscoverSensors
            Layout.alignment: Qt.AlignCenter

            text: qsTr("Discover sensors")
            onClicked: service.discoverSensors()
        }

        Item {
            // padding item
            Layout.preferredHeight: root.height * 0.02
        }
    }
}
