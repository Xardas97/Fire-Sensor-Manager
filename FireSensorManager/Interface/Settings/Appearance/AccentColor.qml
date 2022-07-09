import QtQuick
import QtQuick.Controls

Pane {
    id: root

    property int accentColor
    property int sideLength

    contentHeight: sideLength
    contentWidth: sideLength
    padding: 0

    Material.background: accentColor
    Material.elevation: accentColor === service.uiAccent ? 10 : 0

    MouseArea {
        anchors.fill: parent
        onClicked: service.uiAccent = root.accentColor
    }
}
