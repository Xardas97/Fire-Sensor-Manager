import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Custom.Sensors

Dialog {
    id: root
    title: qsTr("Map Setup")

    standardButtons: Dialog.Close
    //Component.onCompleted:standardButton(Dialog.Save).enabled = Qt.binding(isSensorNameValueValid)

    ColumnLayout {
        id: layout
        anchors.fill: parent

        Label {
            Layout.fillWidth: true

            text: qsTr("Upload map");
            font.underline: true
        }

        UploadMap {
            id: uploadMap
            Layout.fillWidth: true
        }
    }
}
