import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ColumnLayout {
    id: root

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

    ListView {
        Layout.preferredWidth: root.width
        Layout.fillHeight: true

        model: service.knownSensorsModel
        delegate: SensorView {
            sensor:  model.data
            width: root.width
        }
    }
}
