import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Item {
    id: root

    property Sensor selectedSensor
    signal selectedSensorPlaced()

    Flickable {
        id: flickable
        anchors.fill: parent

        boundsMovement: Flickable.StopAtBounds
        clip: true

        contentWidth: imageMap.width * imageMap.scale
        contentHeight: imageMap.height * imageMap.scale

        Image {
            id: imageMap
            transformOrigin: Item.TopLeft
            source: imageSource()

            function imageSource() {
                if (service.selectedFloor == null || service.selectedFloorPart == null)
                    return ""

                return "image://MapImageProvider/" + service.selectedFloor +  "/" + service.selectedFloorPart
            }

        }
        MouseArea {
            anchors.fill: parent
            onClicked: function(mouse) {
                if (selectedSensor != null) {
                    var created = createSensorIconObject(selectedSensor, mouse.x, mouse.y)
                    if (created)
                        selectedSensorPlaced()
                }
            }

            function createSensorIconObject(sensor, x, y) {
                var component = Qt.createComponent("SensorIcon.qml");
                var icon = component.createObject(imageMap, {x: x, y: y, sensor: sensor});

                if (icon === null) {
                    console.log("Error creating object");
                    return false
                }

                var realX = x - icon.width / 2
                var realY = y - icon.height / 2
                icon.x = Math.min(Math.max(realX, 0), imageMap.width - icon.width)
                icon.y = Math.min(Math.max(realY, 0), imageMap.height - icon.height)

                return true
            }
        }
    }

    RowLayout {
        anchors {
            top: root.top
            right: root.right
            margins: 10
        }
        spacing: 0

        visible: imageMap.source != ""

        Button {
            onClicked: imageMap.scale += 0.2

            Layout.preferredWidth: implicitWidth * 0.75
            Layout.preferredHeight: implicitHeight * 0.75

            icon.color: "transparent"
            icon.source: "qrc:/Icons/Plus.png"
            background: Rectangle {
                color: "grey"
                opacity: 0.5
            }
        }
        Button {
            enabled: imageMap.scale >= 0.4
            onClicked: imageMap.scale -= 0.2

            Layout.preferredWidth: implicitWidth * 0.75
            Layout.preferredHeight: implicitHeight * 0.75

            icon.color: "transparent"
            icon.source: "qrc:/Icons/Minus.png"
            background: Rectangle {
                color: "grey"
                opacity: 0.5
            }
        }
        Button {
            onClicked: imageMap.scale = 1

            Layout.preferredWidth: implicitWidth * 0.75
            Layout.preferredHeight: implicitHeight * 0.75

            icon.color: "transparent"
            icon.source: "qrc:/Icons/Reset.png"
            background: Rectangle {
                color: "grey"
                opacity: 0.5
            }
        }
    }
}
