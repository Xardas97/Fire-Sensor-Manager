import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Dialog {
    id: root
    title: qsTr("Appearance")

    standardButtons: Dialog.Close

    ColumnLayout {
        id: layout

        anchors.fill: parent

        Label {
            id: lblTheme
            text: qsTr("Theme:")
        }

        ButtonGroup {
            id: btnGroupThemes
            exclusive: true
            buttons: layoutThemes.children
        }

        RowLayout {
            id: layoutThemes

            CheckBox {
                text: qsTr("Light")
                checked: service.uiTheme === Material.Light
                onClicked: service.uiTheme = Material.Light
            }

            CheckBox {
                text: qsTr("Dark")
                checked: service.uiTheme === Material.Dark
                onClicked: service.uiTheme = Material.Dark
            }

            CheckBox {
                text: qsTr("System Default")
                checked: service.uiTheme === Material.System
                onClicked: service.uiTheme = Material.System
            }
        }

        Label {
            id: lblAccent
            text: qsTr("Accent Color:")
        }

        GridLayout {
            id: gridAccent

            Layout.fillWidth: true

            columns: 4
            property int elementWidth: (width - columnSpacing * (columns - 1)) / columns

            AccentColor {
                accentColor: Material.Pink
                sideLength: gridAccent.elementWidth
            }

            AccentColor {
                accentColor: Material.Orange
                sideLength: gridAccent.elementWidth
            }

            AccentColor {
                accentColor: Material.Purple
                sideLength: gridAccent.elementWidth
            }

            AccentColor {
                accentColor: Material.Green
                sideLength: gridAccent.elementWidth
            }

            AccentColor {
                accentColor: Material.Cyan
                sideLength: gridAccent.elementWidth
            }

            AccentColor {
                accentColor: Material.Blue
                sideLength: gridAccent.elementWidth
            }

            AccentColor {
                accentColor: Material.Yellow
                sideLength: gridAccent.elementWidth
            }

            AccentColor {
                accentColor: Material.Brown
                sideLength: gridAccent.elementWidth
            }
        }
    }
}
