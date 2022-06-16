import QtQuick
import QtQuick.Controls

import "../FloorMaps"

Rectangle {
    id: root

    signal chosenFloorChanged(int floor)

    color: "grey"

    FloorMapControls {
        id: floorMapControls

        anchors.verticalCenter: root.verticalCenter
        anchors.right: root.right
        anchors.rightMargin: 10

        onChosenFloorChanged: function(floor) { root.chosenFloorChanged(floor) }
    }
}
