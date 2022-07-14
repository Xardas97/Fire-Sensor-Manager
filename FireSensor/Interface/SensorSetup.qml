import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs as Dialogs

ColumnLayout {
    id: root

    property int enabledCapabilities: chboxTemperature.checked  | chboxSmoke.checked << 1 | chboxCo2Concentration.checked << 2 | chboxPollution.checked << 3

    property int fontSize

    signal startRequested
    signal loadRequested(url url)

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

    Column {
        Layout.preferredWidth: root.width / 2
        Layout.alignment: Qt.AlignCenter

        Button {
            id: btnStartSensor

            width: parent.width

            enabled: enabledCapabilities

            text: qsTr("Start New Sensor")
            font.pixelSize: fontSize

            onClicked: startRequested()
        }

        Item {
            id: padding
            width: parent.width
            height: btnStartSensor.height / 3
        }

        Button {
            id: btnLoadSensor

            visible: Qt.platform.os !== "android"

            width: parent.width

            text: qsTr("Load Sensor")
            font.pixelSize: fontSize

            onClicked: fileDialogLoad.open()
        }
    }

    Dialogs.FileDialog {
        id: fileDialogLoad

        defaultSuffix: "sns"
        fileMode: Dialogs.FileDialog.OpenFile
        nameFilters: ["Sensor files (*.sns)"]
        currentFolder: StandardPaths.writableLocation(StandardPaths.AppLocalDataLocation)

        onAccepted: loadRequested(fileDialogLoad.selectedFile)
    }
}
