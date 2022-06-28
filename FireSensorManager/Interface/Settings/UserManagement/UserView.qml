import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    property string username
    property bool modPermissions

    Label {
        text: root.username
        elide: Text.ElideRight

        anchors.margins: 5

        ToolTip.text: root.username
        ToolTip.visible: mouseAreaHover.containsMouse
        MouseArea {
            id: mouseAreaHover
            anchors.fill: parent
            hoverEnabled: true
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

    Button {
        Layout.fillWidth: true

        icon.color: "transparent"
        icon.source: "qrc:/Resources/Icons/Remove.png"
        background: Rectangle { opacity: 0 }

        onClicked: service.users.removeUser(root.username)
    }

    Item {
        id: filler
        Layout.preferredWidth: root.width * 0.1
    }
}
