import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

CheckBox {
    id: root
    property int mask
    property string name

    Layout.alignment: Qt.AlignCenter
    text: root.name

    checked: false
    onCheckStateChanged: {
        if (checked) {
            server.sensorState.status = server.sensorState.status | root.mask
        }
        else {
            server.sensorState.status = server.sensorState.status & root.mask
        }
    }
}
