import QtQuick
import QtQuick.Controls

Item {
    id: root
    signal showFloorView()

    Button {
        text: qsTr("Show floor 0")
        anchors.centerIn: parent

        onClicked: {
            service.selectedFloor = 0
            root.showFloorView()
        }
    }
}
