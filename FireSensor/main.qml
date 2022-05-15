import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    minimumWidth: column.width * 1.2
    minimumHeight: column.height + 50
    width: minimumWidth
    height: minimumHeight

    visible: true

    title: qsTr("FireSensor")

    ColumnLayout {
        id: column

        anchors.centerIn: parent

        RowLayout {
            id: temperatureRow

            Layout.alignment: Qt.AlignCenter

            Label {
                id: lbltemperature

                Layout.alignment: Qt.AlignCenter

                text: "Temperature:"
                color: "black"
            }

            SpinBox {
                id: spintemperature

                Layout.alignment: Qt.AlignCenter

                value: server.temperature
                from: -20
                to: 100
                editable: true

                onValueModified: server.temperature = value
            }
        }

    }
}
