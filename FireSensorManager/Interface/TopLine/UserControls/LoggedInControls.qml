import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    Label {
        id: txtUsername
        visible: service.isLoggedIn
        text: service.loggedUsername
    }

    Button {
        id: btnUserIcon
        onClicked: menuUser.open()

        icon.source: getUserIcon()
        background: Rectangle { opacity: 0 }
    }

    function getUserIcon() {
        if (service.hasAdminPermissions)
            return "qrc:/Resources/Icons/Admin.png"

        if (service.hasModPermissions)
            return "qrc:/Resources/Icons/Moderator.png"

        return "qrc:/Resources/Icons/User.png"
    }

    Menu {
        id: menuUser
        width: 80

        x: btnUserIcon.x + btnUserIcon.width / 2 - width
        y: btnUserIcon.y + btnUserIcon.height / 2

        MenuItem {
            text: qsTr("Log out")
            onTriggered: service.logOut()
        }
    }
}
