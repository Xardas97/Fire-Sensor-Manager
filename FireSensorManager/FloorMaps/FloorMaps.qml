import QtQuick
import QtQuick.Controls

Item {
    id: root

    function chosenFloorChanged(floor)
    {
        console.log("Chosen floor changed to: " + floor)
        imageMap.source = "image://MapImageProvider/" + floor +  "/0"
    }

    function noAvailableFloors()
    {
        console.log("No valid floor found")
        imageMap.source = ""
    }

    Image {
        id: imageMap
        anchors.fill: parent
        source: ""
    }
}
