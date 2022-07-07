import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Pane {
    id: root

    background: Rectangle {
        color: "transparent"
    }

    function stopPlaying() {
        if (service.warningTracker.alarmedSensors.length === 0)
                  service.alarmManager.stop()
    }

    ButtonGroup  {
        exclusive: true
        buttons: layout.children

        onClicked: {
            service.alarmManager.play(checkedButton.alarm, 1)
            service.alarmManager.setAlarm(checkedButton.alarm)
        }
    }

    ColumnLayout {
        id: layout

        Repeater {
            model: 5

            CheckBox {
                text: getAlarmName(index);
                property int alarm: index
                Component.onCompleted: checked = service.alarmManager.alarm() === alarm
            }
        }
    }

    function getAlarmName(alarm) {
        switch (alarm) {
            case 0: return qsTr("Classic")
            case 1: return qsTr("Digital Clock")
            case 2: return qsTr("Buzzer")
            case 3: return qsTr("Facility")
            case 4: return qsTr("Vintage")
            default: return qsTr("")
        }
    }
}
