import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Popup {
    id: root

    width: gridAdditionalInfo.width * 1.2
    height: gridAdditionalInfo.height * 1.2

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    GridLayout {
        id: gridAdditionalInfo
        anchors.centerIn: parent

        rows: 3
        columns: 2

        Label {
            Layout.alignment: Qt.AlignRight
            font.pixelSize: fontSize
            text: qsTr("UUID: ")
        }

        Label {
            Layout.alignment: Qt.AlignLeft
            font.pixelSize: fontSize
            text: sensor.sensorState.uuid
        }

        Label {
            Layout.alignment: Qt.AlignRight
            font.pixelSize: fontSize
            text: qsTr("Address: ")
        }
        Label {
            Layout.alignment: Qt.AlignLeft
            font.pixelSize: fontSize
            text: sensor.sensorState.address.toString()
        }

        Label {
            Layout.alignment: Qt.AlignRight
            font.pixelSize: fontSize
            text: qsTr("Port: ")
        }

        Label {
            Layout.alignment: Qt.AlignLeft
            font.pixelSize: fontSize
            text: sensor.sensorState.port
        }
    }
}
