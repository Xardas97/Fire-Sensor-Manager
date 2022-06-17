import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Dialog {
    id: root
    title: qsTr("Map Setup")

    standardButtons: Dialog.Close

    ColumnLayout {
        id: layout

        Label {
            text: qsTr("Upload map");
            font.underline: true
        }

        UploadMap {
            id: uploadMap
        }
    }

    onClosed: uploadMap.errorLabelVisibility = false
}
