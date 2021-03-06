import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Dialog {
    id: root
    title: qsTr("User Management")

    width: 450
    height: 450

    standardButtons: Dialog.Close

    ColumnLayout {
        id: layout
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            Label {
                text: qsTr("Username")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredWidth: layout.width * 0.5
            }

            Label {
                text: qsTr("Moderator")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredWidth: layout.width * 0.2
            }
        }

        Item {
            id: padding
            Layout.fillWidth: true
            Layout.preferredHeight: layout.spacing
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                clip: true
                width: layout.width

                boundsMovement: Flickable.StopAtBounds

                model: service.users
                delegate: UserView {
                    width: layout.width
                    username: model.username
                    modPermissions: model.permissions > 0
                }
            }
        }

        Button {
            Layout.alignment: Qt.AlignHCenter

            anchors.topMargin: layout.spacing * 2

            text: qsTr("Add User")
            onClicked: popupAddUser.open()
        }
    }

    AddUserPopup {
        id: popupAddUser
        anchors.centerIn: parent
    }
}
