import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs as Dialogs

RowLayout {
    id: root

    Label {
        text: qsTr("Floor:")
    }

    SpinBox {
        id: spinFloor

        width: root.width * 0.5

        from: -99
        to: 999

        editable: true
    }

    Button {
        text: qsTr("Upload")
        onClicked: fileDialogUploadMap.open()
    }

    Dialogs.FileDialog {
        id: fileDialogUploadMap

        fileMode: Dialogs.FileDialog.OpenFile
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        currentFolder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)

        onAccepted: {
            var uploaded = service.uploadMap(spinFloor.value, fileDialogUploadMap.selectedFile)
            if (uploaded)
                spinFloor.value = 0
        }
    }
}
