import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Dialog {
    id: root
    title: qsTr("Signalization")
    standardButtons: Dialog.Close

    ColumnLayout {
        id: layout

        AlarmPicker {
            id: alarmPicker
            Layout.fillWidth: true
        }

        VolumePicker {
            id: volumePicker
            Layout.fillWidth: true
        }
    }

    onClosed: alarmPicker.stopPlaying()
}
