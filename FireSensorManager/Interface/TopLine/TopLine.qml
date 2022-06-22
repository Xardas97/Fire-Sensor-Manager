import QtQuick
import QtQuick.Controls

import "../Settings"
import "../FloorMaps"

Rectangle {
    id: root

    signal settingsFlowEntered
    signal settingsFlowLeft

    color: "grey"

    Button {
        id: btnSettingsMenu

        anchors.verticalCenter: root.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: 10

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Menu.png"
        background: Rectangle { opacity: 0 }

        onClicked: popupSettings.open()
    }

    SettingsPopup {
        id: popupSettings
        anchors.centerIn: Overlay.overlay

        onSettingsFlowEntered: root.settingsFlowEntered()
        onSettingsFlowLeft: root.settingsFlowLeft()
    }

    FloorMapControls {
        id: floorMapControls

        anchors.verticalCenter: root.verticalCenter
        anchors.right: root.right
        anchors.rightMargin: 10
    }
}
