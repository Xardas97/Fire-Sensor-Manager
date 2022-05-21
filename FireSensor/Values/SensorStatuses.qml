import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Row {
    id: root

    property int fontSize

    Label {
        id: lblStatus

        height: root.height
        width: root.width * 0.35

        horizontalAlignment: "AlignRight"
        verticalAlignment: "AlignVCenter"

        font.pixelSize: fontSize

        text: qsTr("Statuses:")
        color: "black"
    }

    Item {
        height: root.height
        width: root.width * 0.03
    }

    RowLayout {
        id: rowStatuses

        height: root.height
        width: root.width * 0.62

        SensorStatus {
            Layout.alignment: Qt.AlignLeft

            mask: 1
            name: qsTr("Error")
            font.pixelSize: fontSize
        }

        SensorStatus {
            Layout.alignment: Qt.AlignLeft

            mask: 2
            name: qsTr("Dirty")
            font.pixelSize: fontSize
        }

        SensorStatus {
            Layout.alignment: Qt.AlignLeft

            mask: 4
            name: qsTr("Maintenance")
            font.pixelSize: fontSize
        }
    }
}
