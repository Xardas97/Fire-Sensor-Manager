import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    property alias btnStartSensor: btnStartSensor
    property int enabledCapabilities: chboxTemperature.checked  | chboxSmoke.checked << 1 | chboxCo2Concentration.checked << 2 | chboxPollution.checked << 3

    property int fontSize

    GridLayout {
        id: rowCapabilities

        rows: 2
        columns: 2
        columnSpacing: fontSize

        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter

        CheckBox {
            id: chboxTemperature

            Layout.alignment: Qt.AlignLeft
            text: qsTr("Temperature");
            font.pixelSize: fontSize
        }

        CheckBox {
            id: chboxSmoke

            Layout.alignment: Qt.AlignLeft

            text: qsTr("Smoke Detection");
            font.pixelSize: fontSize
        }

        CheckBox {
            id: chboxCo2Concentration

            Layout.alignment: Qt.AlignLeft
            text: qsTr("CO2 Concentration");
            font.pixelSize: fontSize
        }

        CheckBox {
            id: chboxPollution

            Layout.alignment: Qt.AlignLeft

            text: qsTr("Pollution");
            font.pixelSize: fontSize
        }
    }

    Button {
        id: btnStartSensor

        Layout.preferredWidth: root.width / 2
        Layout.alignment: Qt.AlignCenter

        enabled: enabledCapabilities

        text: qsTr("Start New Sensor")
        font.pixelSize: fontSize
    }
}
