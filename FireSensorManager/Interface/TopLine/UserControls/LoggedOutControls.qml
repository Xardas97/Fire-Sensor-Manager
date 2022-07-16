import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root

    property int fontSize: 13

    Button {
        id: btnLogIn

        Layout.preferredHeight: root.height

        text: qsTr("Log in")
        onClicked: popupLogin.open()
    }

    Popup {
        id: popupLogin

        x: btnLogIn.x + btnLogIn.width - width
        y: btnLogIn.y + btnLogIn.height / 2

        ColumnLayout {
            Keys.onEnterPressed: btnLogin.activate()
            Keys.onReturnPressed: btnLogin.activate()

            Label {
                id: lblLoginError
                text: qsTr("Wrong credentials!")
                font.pointSize: root.fontSize
                color: "red"
            }

            TextField {
                id: txtUsername
                placeholderText: qsTr("username")
                font.pointSize: root.fontSize
                Layout.preferredWidth: lblLoginError.width
            }

            TextField {
                id: txtPassphrase
                placeholderText: qsTr("passphrase")
                font.pointSize: root.fontSize
                echoMode: TextInput.Password
                Layout.preferredWidth: lblLoginError.width
            }

            Button {
                id: btnLogin
                text: qsTr("Login")
                font.pointSize: root.fontSize

                Layout.preferredWidth: lblLoginError.width
                enabled: txtUsername.length > 0 && txtPassphrase.length > 0

                onClicked: activate()
                function activate() {
                    if (!enabled) return

                    var success = service.users.logIn(txtUsername.text, txtPassphrase.text)
                    if (!success) {
                        txtPassphrase.text = ""
                        lblLoginError.visible = true
                        return
                    }

                    popupLogin.close()
                }
            }
        }

        onAboutToShow: {
            txtUsername.text = ""
            txtPassphrase.text = ""
            lblLoginError.visible = false

            txtUsername.forceActiveFocus()
        }
    }
}
