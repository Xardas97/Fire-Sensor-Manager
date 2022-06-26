import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    Button {
        id: btnLogIn
        visible: !service.isLoggedIn

        text: qsTr("Log in")

        onClicked: popupLogin.open()
    }

    Popup {
        id: popupLogin

        x: btnLogIn.x + btnLogIn.width / 2 - width
        y: btnLogIn.y + btnLogIn.height / 2

        ColumnLayout {
            Label {
                id: lblLoginError
                text: qsTr("Wrong credentials!")
                color: "red"
            }

            TextField {
                id: txtUsername
                placeholderText: qsTr("username")
            }

            TextField {
                id: txtPassphrase
                placeholderText: qsTr("passphrase")
                echoMode: TextInput.Password
            }

            Button {
                text: qsTr("Login")

                Layout.fillWidth: true
                enabled: txtUsername.length > 0 && txtPassphrase.length > 0

                onClicked: {
                    var success = service.logIn(txtUsername.text, txtPassphrase.text)
                    if (!success) {
                        lblLoginError.visible = true
                        return
                    }

                    popupLogin.close()
                }
            }
        }

        onOpened: {
            txtUsername.text = ""
            txtPassphrase.text = ""
            lblLoginError.visible = false
        }
    }
}
