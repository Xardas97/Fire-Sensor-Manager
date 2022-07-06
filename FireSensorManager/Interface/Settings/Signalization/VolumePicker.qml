import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Slider {
    id: root

    from: 0
    to: 1.0

    Component.onCompleted: value = service.alarmManager.volume()
    onMoved: service.alarmManager.setVolume(value)
}
