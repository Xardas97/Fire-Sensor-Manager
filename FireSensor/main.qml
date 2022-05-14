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
            id: nextNumberRow

            Layout.alignment: Qt.AlignCenter

            Label {
                id: lblNextNumber

                Layout.alignment: Qt.AlignCenter

                text: "Next Number:"
                color: "black"
            }

            SpinBox {
                id: spinNextNumber

                Layout.alignment: Qt.AlignCenter

                value: server.nextNumber
                from: -100
                to: 100
                editable: true

                onValueModified: server.nextNumber = value
            }
        }

    }
}
