import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "MapSetup"
import "Appearance"
import "Signalization"
import "UserManagement"

Popup {
    id: root

    width: columnSettings.width * 1.2
    height: columnSettings.height * 1.2

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    ColumnLayout {
        id: columnSettings
        anchors.centerIn: parent

        property int fontSize: 17

        spacing: 10

        Button {
            id: btnAppearance

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            enabled: service.users.isLoggedIn

            font.pixelSize: columnSettings.fontSize
            text: qsTr("Appearance")

            onClicked: {
                root.close()
                apperance.open()
            }
        }

        Button {
            id: btnSignalization

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true

            enabled: service.users.isLoggedIn

            font.pixelSize: columnSettings.fontSize
            text: qsTr("Signalization")

            onClicked: {
                root.close()
                dialogSignalization.open()
            }
        }

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

        modal: root.modal

        parent: Overlay.overlay
        anchors.centerIn: parent
    }

    Signalization {
        id: dialogSignalization

        modal: root.modal

        parent: Overlay.overlay
        anchors.centerIn: parent
    }

    UserManagement {
        id: dialogUserManagement

        modal: root.modal

        parent: Overlay.overlay
        anchors.centerIn: parent
    }

    Appearance {
        id: apperance

        modal: root.modal

        parent: Overlay.overlay
        anchors.centerIn: parent
    }
}
