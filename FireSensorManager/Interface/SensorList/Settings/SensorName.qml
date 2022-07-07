import QtQuick
import QtQuick.Controls

import Custom.Sensors

Row {
    id: root

    property string currentName
    property alias value: txtName.text

    onCurrentNameChanged: txtName.text = currentName

    spacing: 20

    Label {
        id: lblName

        width: (root.width - 2 * root.spacing) * 0.35

        anchors.verticalCenter: root.verticalCenter
        horizontalAlignment: "AlignRight"

        text: qsTr("Sensor Name:")
    }

    TextField {
        id: txtName

        width: (root.width - 2 * root.spacing) * 0.55

        anchors {
            topMargin: (root.height - height) / 2
            verticalCenter: root.verticalCenter
        }

        horizontalAlignment: "AlignLeft"
    }

    Button {
        id: btnSettings

        width: (root.width - 2 * root.spacing) * 0.10

        anchors {
            verticalCenter: root.verticalCenter
            topMargin: (root.height - height) / 2
            bottomMargin: (root.height - height) / 2
        }

        icon.source: "qrc:/Resources/Icons/Refresh.png"
        background: Rectangle { opacity: 0 }

        onClicked: txtName.text = currentName
    }
}
