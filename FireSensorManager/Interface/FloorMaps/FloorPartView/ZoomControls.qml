import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root
    spacing: 0

    property Item itemToScale

    Button {
        onClicked: itemToScale.scale += 0.2

        Layout.preferredWidth: implicitWidth * 0.75
        Layout.preferredHeight: implicitHeight * 0.75

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Plus.png"
        background: Rectangle {
            color: "grey"
            opacity: 0.5
        }
    }
    Button {
        enabled: itemToScale.scale >= 0.4
        onClicked: itemToScale.scale -= 0.2

        Layout.preferredWidth: implicitWidth * 0.75
        Layout.preferredHeight: implicitHeight * 0.75

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Minus.png"
        background: Rectangle {
            color: "grey"
            opacity: 0.5
        }
    }
    Button {
        onClicked: itemToScale.scale = 1

        Layout.preferredWidth: implicitWidth * 0.75
        Layout.preferredHeight: implicitHeight * 0.75

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Reset.png"
        background: Rectangle {
            color: "grey"
            opacity: 0.5
        }
    }
}
