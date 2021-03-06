import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Item {
    id: root

    property Sensor selectedSensor
    signal selectedSensorPlaced()

    signal showPreviousView()

    function showSensor(sensor) {
        var iconWidth = 64
        var iconHeight = 64

        var selected = service.selectFloorPartThatContains(sensor)
        if (!selected)
            return

        var centerX = (sensor.x + iconWidth / 2) * imageMap.scale - flickable.width / 2
        var centerY = (sensor.y + iconHeight / 2) * imageMap.scale - flickable.height / 2

        flickable.contentX = (Math.min(Math.max(centerX, 0), flickable.contentWidth - flickable.width))
        flickable.contentY = (Math.min(Math.max(centerY, 0), flickable.contentHeight - flickable.height))
    }

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
            source: ""

            function displaySelectedMap() {
                source = "image://MapImageProvider/" + service.selectedFloor +  "/" + service.selectedFloorPart
            }

            MouseArea {
                anchors.fill: parent
                onClicked: function(mouse) {
                    if (selectedSensor == null)
                        return

                    if (!service.users.hasModPermissions)
                        return

                    service.removeFromMap(selectedSensor)
                    var created = createSensorIconObjectFromMouseEvent(selectedSensor, mouse.x, mouse.y)
                    if (created) {
                        service.placeOnMap(selectedSensor)
                        selectedSensorPlaced()
                    }
                }
            }
        }

        Connections {
            target: service
            function onSelectedFloorChanged() { displaySelectedMap() }
            function onSelectedFloorPartChanged() { displaySelectedMap() }

            function displaySelectedMap() {
                if (!service.selectedFloorPartExists())
                    return;

                imageMap.displaySelectedMap()
                loadSelectedMapSensors()
            }

            function loadSelectedMapSensors() {
                var currentMapSensors = service.currentMapSensors()
                for (var i = 0; i < currentMapSensors.length; ++i) {
                    var sensor = currentMapSensors[i]
                    createSensorIconObject(sensor, sensor.x, sensor.y)
                }
            }
        }
    }

    function createSensorIconObject(sensor, x, y) {
        var component = Qt.createComponent("SensorIcon.qml");
        var icon = component.createObject(imageMap, {x: x, y: y, sensor: sensor});

        if (icon === null) {
            console.log("Error creating object");
            return null
        }

        // workaround for https://bugreports.qt.io/browse/QTBUG-77629
        icon.onPressed.connect(function () { flickable.interactive = false })
        icon.onReleased.connect(function () { flickable.interactive = true })

        return icon
    }

    function createSensorIconObjectFromMouseEvent(sensor, x, y) {
        var icon = createSensorIconObject(sensor, x, y)

        if (icon === null)
            return false

        var realX = x - icon.width / 2
        var realY = y - icon.height / 2
        icon.x = Math.min(Math.max(realX, 0), imageMap.width - icon.width)
        icon.y = Math.min(Math.max(realY, 0), imageMap.height - icon.height)

        return true
    }

    ZoomControls {
        anchors {
            top: root.top
            right: root.right
            margins: 10
        }

        visible: imageMap.source != ""
        itemToScale: imageMap
    }

    Button {
        anchors {
            right: root.right
            bottom: root.bottom
            margins: 10
        }

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Back.png"
        background: Rectangle {
            color: "grey"
            opacity: parent.hovered ? 0.5 : 0.25
        }

        onClicked: root.showPreviousView()
    }
}
