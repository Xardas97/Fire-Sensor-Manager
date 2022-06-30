import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

GridLayout {
    id: root

    signal showFloorPartView()

    columns: getColumnNumber(service.selectedFloorSize)
    property int rows: Math.ceil(service.selectedFloorSize / columns)

    Repeater {
        id: repeaterIcons
        model: service.selectedFloorSize

        property int buttonHeight: root.height / root.rows - root.rowSpacing * root.rows
        property int buttonWidth: root.width / root.columns - root.columnSpacing * root.columns

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
