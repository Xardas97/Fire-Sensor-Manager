import QtQuick
import QtQuick.Controls

Window {
    id: root

    title: qsTr("FireSensorManager")
    visible: true

    width: minimumWidth
    height: minimumHeight
    minimumWidth: lblErrorText.width * 1.3
    minimumHeight: lblErrorText.height * 2.5

    Label {
        id: lblErrorText
        anchors.centerIn: parent

        color: "red"
        text: qsTr("Failed to start, could not open the database!")
    }
}
