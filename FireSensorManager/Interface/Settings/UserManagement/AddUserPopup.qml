import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Popup {
    id: root

    ColumnLayout {
        id: layout
        Keys.onEnterPressed: btnAddUser.activate()
        Keys.onReturnPressed: btnAddUser.activate()

        Label {
            id: txtUserAlreadyExistsError
            text: qsTr("User already exists!")
            color: "red"
        }

        Label {
            id: txtPassphrasesDifferentError
            text: qsTr("Passphrases don't match!")
            color: "red"
        }

        TextField {
            id: txtUsername
            placeholderText: qsTr("Username")
            Layout.preferredWidth: txtPassphrasesDifferentError.width
        }

        TextField {
            id: txtPassphrase
            echoMode: TextInput.Password
            placeholderText: qsTr("Passhrase")
            Layout.preferredWidth: txtPassphrasesDifferentError.width
        }

        TextField {
            id: txtPassphrase2
            echoMode: TextInput.Password
            placeholderText: qsTr("Repeat Passphrase")
            Layout.preferredWidth: txtPassphrasesDifferentError.width
        }

        CheckBox {
            id: chboxModerator
            text:  qsTr("Moderator")
            Layout.preferredWidth: txtPassphrasesDifferentError.width
        }

        Button {
            id: btnAddUser

            Layout.fillWidth: true

            text: qsTr("Add")
            enabled: txtUsername.text.length > 0 && txtPassphrase.text.length > 0

            onClicked: activate()
            function activate() {
                if (!enabled) return

                if (txtPassphrase.text != txtPassphrase2.text) {
                    layout.resetFields(false)
                    txtPassphrasesDifferentError.visible = true
                    return
                }

                var permissions = chboxModerator.checked ? 1 : 0
                var added = service.users.addUser(txtUsername.text, txtPassphrase.text, permissions);

                if (!added) {
                    layout.resetFields(true)
                    txtUserAlreadyExistsError.visible = true
                    return
                }

                root.close()
            }
        }

        function resetFields(resetUsername) {
            txtUserAlreadyExistsError.visible = false
            txtPassphrasesDifferentError.visible = false
            txtPassphrase.text = ""
            txtPassphrase2.text = ""
            chboxModerator.checked = false

            if (resetUsername)
                txtUsername.text = ""
        }
    }

    onAboutToShow: {
        layout.resetFields(true);
        txtUsername.forceActiveFocus()
    }
}
