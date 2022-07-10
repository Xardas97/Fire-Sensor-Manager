import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Popup {
    id: root

    ColumnLayout {
        id: layout

        RowLayout {
            id: layoutPlacementFilters

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft

            CheckBox {
                id: chboxPlacedFilter
                Layout.alignment: Qt.AlignLeft

                text: qsTr("Placed");

                checked: service.knownSensorsModel.placedFilterEnabled
                onCheckStateChanged: {
                    service.knownSensorsModel.placedFilterEnabled = checked
                    if (checked) chboxUnplacedFilter.checked = false
                }
            }

            CheckBox {
                id: chboxUnplacedFilter
                Layout.alignment: Qt.AlignLeft

                text: qsTr("Unplaced");

                checked: service.knownSensorsModel.unplacedFilterEnabled
                onCheckStateChanged: {
                    service.knownSensorsModel.unplacedFilterEnabled = checked
                    if (checked) chboxPlacedFilter.checked = false
                }
            }
        }

        CheckBox {
            id: chboxActiveFilter

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft

            text: qsTr("Active");

            checked: service.knownSensorsModel.activeFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.activeFilterEnabled = checked
        }

        CheckBox {
            id: chboxAlarmFilter

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft

            text: qsTr("Alarmed");

            checked: service.knownSensorsModel.alarmedFilterEnabled
            onCheckStateChanged: service.knownSensorsModel.alarmedFilterEnabled = checked
        }
    }
}
