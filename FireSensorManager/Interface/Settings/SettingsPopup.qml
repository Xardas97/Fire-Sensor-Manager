import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "MapSetup"
import "Signalization"
import "UserManagement"

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
            id: btnMapSetup

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            enabled: service.users.hasModPermissions

            font.pixelSize: columnSettings.fontSize
            text: qsTr("Map Setup")

            onClicked: {
                root.close()
                dialogMapSetup.open()
            }
        }

        Button {
            id: btnSignalization

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            enabled: service.users.hasModPermissions

            font.pixelSize: columnSettings.fontSize
            text: qsTr("Signalization")

            onClicked: {
                root.close()
                dialogSignalization.open()
            }
        }

        Button {
            id: btnUserManagement

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            enabled: service.users.hasAdminPermissions

            font.pixelSize: columnSettings.fontSize
            text: qsTr("User Management")

            onClicked:  {
                root.close()
                dialogUserManagement.open()
            }
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

    Signalization {
        id: dialogSignalization
        parent: Overlay.overlay

        anchors.centerIn: parent
        onOpened: settingsFlowEntered()
        onClosed: settingsFlowLeft()
    }

    UserManagement {
        id: dialogUserManagement
        parent: Overlay.overlay

        anchors.centerIn: parent
        onOpened: settingsFlowEntered()
        onClosed: settingsFlowLeft()
    }
}
