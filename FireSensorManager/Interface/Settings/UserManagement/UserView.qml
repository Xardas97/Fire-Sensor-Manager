import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../../Controls"

RowLayout {
    id: root

    property string username
    property bool modPermissions

    Label {
        id: lblUsername

        font.pointSize: 13
        text: root.username
        elide: Text.ElideRight

        anchors.margins: 5

        MouseArea {
            id: mouseAreaTooltip
            hoverEnabled: true
            anchors.fill: parent
            acceptedButtons: Qt.NoButton

            ToolTip {
                font.pointSize: 13
                text: lblUsername.text
                visible: mouseAreaTooltip.containsMouse && lblUsername.truncated
            }
        }

        horizontalAlignment: Text.AlignHCenter
        Layout.preferredWidth: root.width * 0.5
    }

    Item {
        Layout.fillHeight: true
        Layout.preferredWidth: root.width * 0.2

        CheckBox {
            id: chboxPermissions
            anchors.centerIn: parent

            Component.onCompleted: checked = root.modPermissions
            onCheckStateChanged: {
                var newPermissions = checked ? 1 : 0
                service.users.updateUserPermissions(root.username, newPermissions)
            }
        }
    }

    IconButton {
        Layout.fillWidth: true
        icon.source: "qrc:/Resources/Icons/Remove.png"
        onClicked: dialogRemoveUser.open()
    }

    Item {
        id: filler
        Layout.preferredWidth: root.width * 0.1
    }

    Dialog {
        id: dialogRemoveUser
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: service.users.removeUser(root.username)

        Label {
            font.pointSize: 13
            text: qsTr("Are you sure you want to remove user '%1'?").arg(username)
        }
    }
}
