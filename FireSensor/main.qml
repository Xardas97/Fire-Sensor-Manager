import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("FireSensor")

    ColumnLayout {
        anchors.fill: parent

        Label {
            id: txt

            Layout.alignment: Qt.AlignCenter

            text: "Just a random text"
            color: "purple"
        }
    }
}
