import QtQuick
import QtQuick.Layouts
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

    Flickable {
        anchors.fill: parent

        boundsMovement: Flickable.StopAtBounds
        clip: true

        contentWidth: imageMap.width * imageMap.scale
        contentHeight: imageMap.height * imageMap.scale

        Image {
            id: imageMap
            transformOrigin: Item.TopLeft
            source: ""
        }
    }

    RowLayout {
        anchors {
            top: root.top
            right: root.right
            margins: 10
        }

        spacing: 0

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
