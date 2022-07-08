import QtQuick
import QtQuick.Controls

Button {
    background.visible: false
    onHoveredChanged: background.visible = hovered
}
