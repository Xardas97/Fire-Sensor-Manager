import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "Controls"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("TestApp")

    ColumnLayout {
        anchors.fill: parent

        Label {
            id: txt

            Layout.alignment: Qt.AlignCenter

            text: "Just a random text"
            color: "purple"
        }

        Button {
            id: btn

            Layout.alignment: Qt.AlignCenter

            text: "Click me!"

            onClicked: txt.text = "You clicked this " + service.getNextNumber() + " times!"
        }
    }
}
