import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

import "../Controls"

Rectangle {
    id: root

    property Sensor selectedSensor: null

    signal requestShowSensor(Sensor sensor)

    color: "transparent"
    border {
        color: "black"
        width: 1
    }

    ColumnLayout {
        id: column

        spacing: 0
        anchors.fill: parent

        Item {
            id: itemFilters

            Layout.alignment: Qt.AlignRight
            width: btnOpenPopupFilters.width
            height: btnOpenPopupFilters.height

            IconButton {
                id: btnOpenPopupFilters
                icon.source: "qrc:/Resources/Icons/Next.png"
                onClicked: popupFilters.open()
            }

            IconButton {
                id: btnClosePopupFilters
                icon.source: "qrc:/Resources/Icons/Previous.png"
                visible: !btnOpenPopupFilters.visible
                onClicked: popupFilters.close()
            }

            SensorListFiltersPopup {
                id: popupFilters

                x: btnOpenPopupFilters.x + btnOpenPopupFilters.width + 10
                y: btnOpenPopupFilters.y + btnOpenPopupFilters.height / 2

                onAboutToShow: btnOpenPopupFilters.visible = false
                onAboutToHide: btnOpenPopupFilters.visible = true
            }
        }

        ScrollView {
            id: scrollSensors
            Layout.fillWidth: true
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
            Layout.fillWidth: true
            Layout.preferredHeight: column.height * 0.02
        }

        Button {
            id: btnDiscoverSensors
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: column.width * 0.8

            text: qsTr("Discover sensors")
            onClicked: service.discoverSensors()
        }

        Item {
            id: itemPadding2
            Layout.fillWidth: true
            Layout.preferredHeight: column.height * 0.02
        }
    }
}
