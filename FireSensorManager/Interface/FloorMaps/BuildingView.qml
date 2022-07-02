import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Flickable {
    id: root
    signal showFloorView()

    clip: true
    boundsMovement: Flickable.StopAtBounds

    contentWidth: contentWrapper.width
    contentHeight: contentWrapper.height

    contentY: contentWrapper.height - root.height

    Item {
        id: contentWrapper

        width: Math.max(root.width, layout.width)
        height: Math.max(root.height, layout.height)

        ColumnLayout {
            id: layout
            width: root.width
            anchors.centerIn: parent

            spacing: 0

            Repeater {
                id: repeater
                model: numberOfFloors(service.availableFloors)
                delegate: Button {
                    id: btn
                    Layout.preferredWidth: root.width
                    Layout.preferredHeight: root.height / 7

                    property int floor: repeater.lowestFloor(service.availableFloors) - index

                    enabled: service.floorExists(floor)

                    text: floor
                    contentItem: Text {
                        text: btn.text
                        font.pointSize: 100
                        fontSizeMode: Text.Fit
                        opacity: enabled ? 1.0 : 0.3
                        horizontalAlignment: Text.AlignHCenter
                    }

                    background: Rectangle {
                        border.width: 1
                        color: parent.enabled ? "lightgrey" : "transparent"
                    }

                    onClicked: {
                        service.selectedFloor = floor
                        root.showFloorView()
                    }
                }

                function numberOfFloors(availableFloors) {
                    var first = parseInt(availableFloors[0])
                    var last = parseInt(availableFloors[availableFloors.length - 1])
                    return last - first + 1
                }

                function lowestFloor(availableFloors) {
                    return parseInt(availableFloors[availableFloors.length - 1])
                }
            }
        }
    }
}
