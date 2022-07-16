import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Dialog {
    id: root
    title: qsTr("Map Setup")

    standardButtons: Dialog.Close

    ColumnLayout {
        id: layout

        UploadMap {
            id: uploadMap
            Layout.fillWidth: true
        }

        RemoveFloor {
            id: removeFloor
            Layout.fillWidth: true
        }
    }

    onClosed: uploadMap.errorLabelVisibility = false
}
