import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {

    border {
        color: "black"
        width: 1
    }

    ColumnLayout {
        id: root

        anchors.fill: parent

        CheckBox {
            id: chboxReplaceFilter

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Replaced Sensors");

            checked: !service.knownSensorsModel.replaceFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.replaceFilterEnabled = !checked
        }

        CheckBox {
            id: chboxActiveFilter

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Inactive Sensors");

            checked: !service.knownSensorsModel.activeFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.activeFilterEnabled = !checked
        }

        ScrollView {
            Layout.preferredWidth: root.width
            Layout.fillHeight: true

            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                width: parent.width
                clip: true

                boundsMovement: Flickable.StopAtBounds

                model: service.knownSensorsModel
                delegate: SensorView {
                    sensor:  model.data
                    width: root.width
                    height: root.height / 10
                }
            }
        }
    }
}
