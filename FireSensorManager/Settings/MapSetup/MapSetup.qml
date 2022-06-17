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
        }

        RemoveFloor {
            id: removeFloor
        }
    }

    onClosed: uploadMap.errorLabelVisibility = false
}
