import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "MapSetup"

Popup {
    id: root

    signal settingsFlowEntered
    signal settingsFlowLeft

    width: columnSettings.width * 1.2
    height: columnSettings.height * 1.2

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    onOpened: settingsFlowEntered()
    onClosed: settingsFlowLeft()

    ColumnLayout {
        id: columnSettings
        anchors.centerIn: parent

        property int fontSize: 17

        spacing: 10

        Button {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            font.pixelSize: columnSettings.fontSize
            text: qsTr("Map Setup")

            onClicked: {
                root.close()
                dialogMapSetup.open()
            }
        }

        Button {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            enabled: false

            font.pixelSize: columnSettings.fontSize
            text: qsTr("Signalization")

            onClicked: console.log("Signalization settings requested")
        }

        Button {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            enabled: false

            font.pixelSize: columnSettings.fontSize
            text: qsTr("User Management")

            onClicked: console.log("User Management settings requested")
        }

        Item {
            id: filler
            Layout.fillWidth: true
            Layout.preferredHeight: 1
        }

        Button {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            font.pixelSize: columnSettings.fontSize
            text: qsTr("Close")

            onClicked: root.close()
        }
    }

    MapSetup {
        id: dialogMapSetup
        parent: Overlay.overlay

        anchors.centerIn: parent
        onOpened: settingsFlowEntered()
        onClosed: settingsFlowLeft()
    }
}
