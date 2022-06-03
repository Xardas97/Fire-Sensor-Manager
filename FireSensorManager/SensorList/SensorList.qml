import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ListView {
    id: root

    model: service.knownSensorsModel
    delegate: SensorView {
        sensor:  model.data
        width: root.width
    }
}
