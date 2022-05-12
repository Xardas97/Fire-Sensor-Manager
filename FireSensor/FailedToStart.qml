import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: txt.width * 1.5
    height: txt.height * 4
    visible: true
    title: qsTr("FireSensor")

    Label {
        id: txt

        anchors.centerIn: parent;

        text: "FAILED TO START, NO PORT AVAILABLE"
        color: "red"
    }
}
