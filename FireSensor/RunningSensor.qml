import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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
}
