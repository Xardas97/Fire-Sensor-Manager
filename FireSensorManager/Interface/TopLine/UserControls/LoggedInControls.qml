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

        icon.source: getUserIcon()
        background: Rectangle { opacity: 0 }

        onClicked: menuUser.open()
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
        width: 140

        x: btnUserIcon.x + btnUserIcon.width / 2 - width
        y: btnUserIcon.y + btnUserIcon.height / 2

        MenuItem {
            text: qsTr("Change passphrase")
            onTriggered: popupChangePassphrase.open()
        }
        MenuItem {
            text: qsTr("Log out")
            onTriggered: service.logOut()
        }
    }

    Popup {
        id: popupChangePassphrase

        x: btnUserIcon.x + btnUserIcon.width / 2 - width
        y: btnUserIcon.y + btnUserIcon.height / 2

        ColumnLayout {
            Label {
                id: lblAuthError
                text: qsTr("Wrong credentials!")
                color: "red"
            }
            Label {
                id: lblNewPassphrasesError
                text: qsTr("Passphrases don't match!")
                color: "red"
            }

            TextField {
                id: txtOldPassphrase

                Layout.preferredWidth: lblNewPassphrasesError.width

                placeholderText: qsTr("Old Passphrase")
                echoMode: TextInput.Password
            }

            TextField {
                id: txtNewPassphrase

                Layout.preferredWidth: lblNewPassphrasesError.width

                placeholderText: qsTr("New Passphrase")
                echoMode: TextInput.Password
            }

            TextField {
                id: txtNewPassphrase2

                Layout.preferredWidth: lblNewPassphrasesError.width

                placeholderText: qsTr("Repeat Passphrase")
                echoMode: TextInput.Password
            }

            Button {
                text: qsTr("Change Passphrase")

                Layout.fillWidth: true
                enabled: txtOldPassphrase.length > 0 && txtNewPassphrase.length > 0 && txtNewPassphrase2.length > 0

                onClicked: {
                    if (txtNewPassphrase.text != txtNewPassphrase2.text) {
                        lblNewPassphrasesError.visible = true
                        lblAuthError.visible = false;
                        return
                    }

                    var success = service.updateUserPassphrase(service.loggedUsername, txtOldPassphrase.text, txtNewPassphrase.text)
                    if (!success) {
                        lblNewPassphrasesError.visible = false
                        lblAuthError.visible = true;
                        return
                    }

                    popupChangePassphrase.close()
                }
            }
        }

        onOpened: {
            txtOldPassphrase.text = ""
            txtNewPassphrase.text = ""
            txtNewPassphrase2.text = ""

            lblAuthError.visible = false
            lblNewPassphrasesError.visible = false
        }
    }
}
