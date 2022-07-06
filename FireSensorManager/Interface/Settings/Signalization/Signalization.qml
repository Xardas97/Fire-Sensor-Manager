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
        }

        VolumePicker {
            id: volumePicker
        }
    }

    onClosed: alarmPicker.stopPlaying()
}
