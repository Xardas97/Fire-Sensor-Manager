import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs as Dialogs

ColumnLayout {
    id: root

    property bool errorLabelVisibility: false

    Label {
        text: qsTr("Upload map");
        font.underline: true
    }

    RowLayout {
        Label {
            text: qsTr("Floor:")
        }

        SpinBox {
            id: spinFloor

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
                if (uploaded) {
                    errorLabelVisibility = false
                    spinFloor.value = 0
                }
                else {
                    errorLabelVisibility = true
                }
            }
        }
    }

    Label {
        id: lblUploadFailed
        Layout.alignment: Qt.AlignRight

        opacity: errorLabelVisibility ? 1 : 0

        text: qsTr("Upload failed!")
        color: "red"
    }
}
