import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

MouseArea {
    id: root
    hoverEnabled: true
    acceptedButtons: Qt.NoButton

    property Sensor sensor
    property bool toolTipTurnedOff: false

    ToolTip {
        delay: 1000
        visible: root.containsMouse && !toolTipTurnedOff

        width: columnText.width * 1.2
        height: columnText.height * 1.2

        ColumnLayout {
            id: columnText
            anchors.centerIn: parent

            Label {
                id: lblName
                text: sensor.name
                font.underline: true
            }

            Label {
                id: lblError

                visible: sensor.status & 1

                text: qsTr("Sensor is reporting an error!")
                color: "red"
            }

            Label {
                id: lblDirty

                visible: sensor.status & 2

                text: qsTr("Sensor is dirty!")
                color: "red"
            }

            Label {
                id: lblMaintenance

                visible: sensor.status & 4

                text: qsTr("Sensor requires maintenance!")
                color: "red"
            }

            RowLayout {
                id: rowTemperature
                visible: sensor.capabilities & 1

                Label {
                    text: qsTr("Temperature: ")
                }

                Label {
                    text: sensor.temperature + "/" + sensor.temperatureThreshold
                    color: sensor.temperature > sensor.temperatureThreshold ? "red" : "black"
                }
            }

            RowLayout {
                id: rowSmokeDetected
                visible: sensor.capabilities & 2

                Label {
                    text: qsTr("Smoke Detected: ")
                }

                Label {
                    text: sensor.smokeDetected ? qsTr("Yes") : qsTr("No")
                    color: sensor.smokeDetected ? "red" : "black"
                }
            }

            RowLayout {
                id: rowCo2Concentration
                visible: sensor.capabilities & 4

                Label {
                    text: qsTr("CO2 Conc: ")
                }

                Label {
                    text: sensor.co2Concentration + "/" + sensor.co2ConcentrationThreshold
                    color: sensor.co2Concentration > sensor.co2ConcentrationThreshold ? "red" : "black"
                }
            }

            RowLayout {
                id: rowPollution
                visible: sensor.capabilities & 8

                Label {
                    text: qsTr("Pollution: ")
                }

                Label {
                    text: sensor.pollution + "/" + sensor.pollutionThreshold
                    color: sensor.pollution > sensor.pollutionThreshold ? "red" : "black"
                }
            }
        }
    }
}
