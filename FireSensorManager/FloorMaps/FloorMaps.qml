import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int chosenFloor: 0

    Image {
        anchors.fill: parent
        source: "image://MapImageProvider/" + chosenFloor +  "/0"
    }
}
