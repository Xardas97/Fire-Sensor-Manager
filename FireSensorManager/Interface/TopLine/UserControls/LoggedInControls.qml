import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    Label {
        id: txtUsername
        text: service.users.loggedUsername
    }

    Button {
        id: btnUserIcon

        icon.source: getUserIcon()
        background: Rectangle { opacity: 0 }

        onClicked: menuUser.open()
    }

    function getUserIcon() {
        if (service.users.hasAdminPermissions)
            return "qrc:/Resources/Icons/Admin.png"

        if (service.users.hasModPermissions)
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
            onTriggered: service.users.logOut()
        }
    }

    Popup {
        id: popupChangePassphrase

        x: btnUserIcon.x + btnUserIcon.width / 2 - width
        y: btnUserIcon.y + btnUserIcon.height / 2

        ColumnLayout {
            id: layout
            Keys.onEnterPressed: btnChangePassphrase.activate()
            Keys.onReturnPressed: btnChangePassphrase.activate()

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
                id: btnChangePassphrase
                text: qsTr("Change Passphrase")

                Layout.fillWidth: true
                enabled: txtOldPassphrase.length > 0 && txtNewPassphrase.length > 0 && txtNewPassphrase2.length > 0

                onClicked: activate()
                function activate() {
                    if (!enabled) return

                    if (txtNewPassphrase.text != txtNewPassphrase2.text) {
                        layout.resetState()
                        lblNewPassphrasesError.visible = true
                        return
                    }

                    var success = service.users.updateUserPassphrase(service.users.loggedUsername, txtOldPassphrase.text, txtNewPassphrase.text)
                    if (!success) {
                        layout.resetState()
                        lblAuthError.visible = true;
                        return
                    }

                    popupChangePassphrase.close()
                }
            }

            function resetState() {
                txtOldPassphrase.text = ""
                txtNewPassphrase.text = ""
                txtNewPassphrase2.text = ""

                lblAuthError.visible = false
                lblNewPassphrasesError.visible = false

                txtOldPassphrase.forceActiveFocus()
            }
        }

        onOpened: {
            layout.resetState()
        }
    }
}
