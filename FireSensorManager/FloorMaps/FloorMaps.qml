import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root

    Flickable {
        anchors.fill: parent

        boundsMovement: Flickable.StopAtBounds
        clip: true

        contentWidth: imageMap.width * imageMap.scale
        contentHeight: imageMap.height * imageMap.scale

        Image {
            id: imageMap
            transformOrigin: Item.TopLeft
            source: imageSource()
        }
    }

    function imageSource() {
        if (service.selectedFloor == null || service.selectedFloorPart == null)
            return ""

        return "image://MapImageProvider/" + service.selectedFloor +  "/" + service.selectedFloorPart
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
