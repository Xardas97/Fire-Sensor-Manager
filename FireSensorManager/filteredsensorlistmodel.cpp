#include "filteredsensorlistmodel.h"

#include "Communication/sensor.h"
#include "Communication/sensorlist.h"

FilteredSensorListModel::FilteredSensorListModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool FilteredSensorListModel::getReplaceFilterEnabled() const
{
    return replaceFilterEnabled;
}

void FilteredSensorListModel::setReplaceFilterEnabled(bool replaceFilterEnabled)
{
    if (this->replaceFilterEnabled == replaceFilterEnabled)
        return;

    this->replaceFilterEnabled = replaceFilterEnabled;

    emit replaceFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::getActiveFilterEnabled() const
{
    return activeFilterEnabled;
}

void FilteredSensorListModel::setActiveFilterEnabled(bool activeFilterEnabled)
{
    if (this->activeFilterEnabled == activeFilterEnabled)
        return;

    this->activeFilterEnabled = activeFilterEnabled;

    emit activeFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!replaceFilterEnabled && !activeFilterEnabled)
        return true;

    const auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    const auto data = index.data(SensorList::Roles::DataRole);
    const Sensor* sensor = data.value<Sensor*>();

    return (!replaceFilterEnabled || !sensor->getIsReplaced()) &&
           (!activeFilterEnabled  || sensor->getIsActive());
}
