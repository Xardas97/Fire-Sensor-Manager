import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Rectangle {
    id: root

    border {
        color: "black"
        width: 1
    }

    property Sensor selectedSensor: null

    signal requestShowSensor(Sensor sensor)

    ColumnLayout {
        id: column

        width: parent.width - 2
        height: parent.height - 2
        anchors.centerIn: parent

        ColumnLayout {
            id: layoutFilters

            Label {
                text: qsTr("Filters:")
            }

            RowLayout {
                id: layoutPlacementFilters
                Layout.alignment: Qt.AlignLeft

                CheckBox {
                    id: chboxPlacedFilter

                    text: qsTr("Placed");

                    checked: service.knownSensorsModel.placedFilterEnabled
                    onCheckStateChanged: {
                        service.knownSensorsModel.placedFilterEnabled = checked
                        if (checked) chboxUnplacedFilter.checked = false
                    }
                }

                CheckBox {
                    id: chboxUnplacedFilter

                    text: qsTr("Unplaced");

                    checked: service.knownSensorsModel.unplacedFilterEnabled
                    onCheckStateChanged: {
                        service.knownSensorsModel.unplacedFilterEnabled = checked
                        if (checked) chboxPlacedFilter.checked = false
                    }
                }
            }

            CheckBox {
                id: chboxActiveFilter

                Layout.alignment: Qt.AlignLeft
                text: qsTr("Active");

                checked: service.knownSensorsModel.activeFilterEnabled
                onCheckStateChanged: service.knownSensorsModel.activeFilterEnabled = checked
            }

            CheckBox {
                id: chboxAlarmFilter

                Layout.alignment: Qt.AlignLeft
                text: qsTr("Alarmed");

                checked: service.knownSensorsModel.alarmedFilterEnabled
                onCheckStateChanged: service.knownSensorsModel.alarmedFilterEnabled = checked
            }
        }

        ScrollView {
            id: scrollSensors
            Layout.preferredWidth: column.width
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
                    width: column.width
                    height: column.height / 10

                    isSelected: sensor == selectedSensor
                    onSelected: selectedSensor = sensor
                    onDeselected: selectedSensor = null

                    onRequestShowSensor: root.requestShowSensor(sensor)
                }
            }
        }

        Item {
            id: itemPadding
            Layout.preferredHeight: column.height * 0.02
        }

        Button {
            id: btnDiscoverSensors
            Layout.alignment: Qt.AlignCenter

            text: qsTr("Discover sensors")
            onClicked: service.discoverSensors()
        }

        Item {
            id: itemPadding2
            Layout.preferredHeight: column.height * 0.02
        }
    }
}
