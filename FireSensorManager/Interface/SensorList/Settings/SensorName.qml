import QtQuick
import QtQuick.Controls

import Custom.Sensors

Item {
    id: root

    property string currentName
    property alias value: txtName.text

    onCurrentNameChanged: txtName.text = currentName

    Label {
        id: lblName

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        text: qsTr("Sensor Name:")
        color: "black"
    }

    TextField {
        id: txtName

        width: root.width * 0.51

        anchors {
            left: lblName.right
            leftMargin: root.width * 0.03
            top: root.top
            topMargin: (root.height - height) / 2
        }

        horizontalAlignment: "AlignLeft"
    }

    Button {
        id: btnSettings

        height: root.height
        width: root.width * 0.10

        anchors {
            left: txtName.right
            leftMargin: root.width * 0.01

            top: root.top
            topMargin: (root.height - height) / 2

            bottom: root.bottom
            bottomMargin: (root.height - height) / 2
        }

        icon.source: "qrc:/Resources/Icons/Refresh.png"
        background: Rectangle { opacity: 0 }

        onClicked: txtName.text = currentName
    }
}
