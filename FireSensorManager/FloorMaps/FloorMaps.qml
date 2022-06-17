import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int chosenFloor: 0

    onChosenFloorChanged: imageMap.source = "image://MapImageProvider/" + chosenFloor +  "/0"

    Image {
        id: imageMap
        anchors.fill: parent
        source: ""
    }
}
