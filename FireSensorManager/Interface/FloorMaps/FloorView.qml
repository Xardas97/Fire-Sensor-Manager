import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root

    signal showFloorPartView()
    signal showPreviousView()

    GridLayout {
        id: grid
        anchors.fill: parent

        columns: getColumnNumber(service.selectedFloorSize)
        property int rows: Math.ceil(service.selectedFloorSize / columns)

        Repeater {
            id: repeaterIcons
            model: service.selectedFloorSize

            property int buttonHeight: grid.height / grid.rows - grid.rowSpacing * grid.rows
            property int buttonWidth: grid.width / grid.columns - grid.columnSpacing * grid.columns

            Button {
                Layout.preferredWidth: repeaterIcons.buttonWidth
                Layout.preferredHeight: repeaterIcons.buttonHeight

                icon {
                    width: repeaterIcons.buttonWidth
                    height: repeaterIcons.buttonHeight

                    color: "transparent"
                    source: getIconSource()
                }

                Component.onCompleted: {
                    if (Qt.platform.os === "windows")
                        background.opacity = 0
                }

                onClicked: {
                    service.selectedFloorPart = index
                    root.showFloorPartView()
                }

                function getIconSource() {
                    if (!service.floorPartExists(service.selectedFloor, index))
                        return ""

                    return "image://MapImageProvider/" + service.selectedFloor +  "/" + index
                }
            }
        }

        function getColumnNumber(itemCount) {
            if (itemCount > 2)
                return Math.round(Math.sqrt(itemCount))

            return itemCount
        }
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
            opacity: 0.5
        }

        onClicked: root.showPreviousView()
    }
}
