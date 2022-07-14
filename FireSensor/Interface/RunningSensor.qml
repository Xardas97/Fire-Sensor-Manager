import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs as Dialogs

import "Values"

Column {
    id: root

    property int fontSize
    signal closeRequested

    DynamicSensorValues {
        id: dynamicSensorValues

        width: root.width
        height: root.height * 0.8

        fontSize: root.fontSize
    }

    Item {
        width: root.width
        height: root.height * 0.1
    }

    RowLayout {
        id: controlButtons

        width: root.width
        height: root.height * 0.1

        Button {
            id: btnAdditionalInfo

            Layout.preferredHeight: controlButtons.height
            Layout.preferredWidth: controlButtons.width * 0.3
            Layout.alignment: Qt.AlignCenter

            font.pixelSize: fontSize

            text: qsTr("Additional Info")
            onClicked: popupAdditionalInfo.open()
        }

        Button {
            id: btnSave

            visible: Qt.platform.os !== "android"

            Layout.preferredHeight: controlButtons.height
            Layout.preferredWidth: controlButtons.width * 0.3
            Layout.alignment: Qt.AlignCenter

            font.pixelSize: fontSize

            text: qsTr("Save")
            onClicked: fileDialogSave.open()
        }

        Button {
            id: btnClose

            Layout.preferredHeight: controlButtons.height
            Layout.preferredWidth: controlButtons.width * 0.3
            Layout.alignment: Qt.AlignCenter

            font.pixelSize: fontSize

            text: qsTr("Close")
            onClicked: root.closeRequested()
        }
    }

    AdditionalInfoPopup {
        id: popupAdditionalInfo
        anchors.centerIn: parent

        onOpened: root.opacity = 0.3
        onClosed: root.opacity = 1
    }

    Dialogs.FileDialog {
        id: fileDialogSave

        defaultSuffix: "sns"
        fileMode: Dialogs.FileDialog.SaveFile
        nameFilters: ["Sensor files (*.sns)"]
        currentFolder: StandardPaths.writableLocation(StandardPaths.AppLocalDataLocation)

        onAccepted: {
            var succ = sensor.saveSensor(fileDialogSave.selectedFile)
            if (!succ) messageDialogFailedSave.open()
        }
    }

    Dialog {
        id: messageDialogFailedSave

        anchors.centerIn: root
        width: lblDialogText.width * 1.2

        modal: true
        focus: true
        standardButtons: Dialog.Ok
        closePolicy: Popup.NoAutoClose

        title: qsTr("Error")
        font.pixelSize: root.fontSize

        Label {
            id: lblDialogText
            text: qsTr("Failed to save sensor data at the chosen location!")
            font.pixelSize: root.fontSize
        }

        onOpened: root.opacity = 0.3
        onClosed: root.opacity = 1
    }
}
