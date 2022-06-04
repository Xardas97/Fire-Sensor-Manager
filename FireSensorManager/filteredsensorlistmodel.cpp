#include "filteredsensorlistmodel.h"

#include "Communication/sensor.h"
#include "Communication/sensorlist.h"

FilteredSensorListModel::FilteredSensorListModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool FilteredSensorListModel::replaceFilterEnabled() const
{
    return m_replaceFilterEnabled;
}

void FilteredSensorListModel::setReplaceFilterEnabled(bool replaceFilterEnabled)
{
    if (m_replaceFilterEnabled == replaceFilterEnabled)
        return;

    m_replaceFilterEnabled = replaceFilterEnabled;

    emit replaceFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::activeFilterEnabled() const
{
    return m_activeFilterEnabled;
}

void FilteredSensorListModel::setActiveFilterEnabled(bool activeFilterEnabled)
{
    if (m_activeFilterEnabled == activeFilterEnabled)
        return;

    m_activeFilterEnabled = activeFilterEnabled;

    emit activeFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_replaceFilterEnabled && !m_activeFilterEnabled)
        return true;

    const auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    const auto data = index.data(SensorList::Roles::DataRole);
    const Sensor* sensor = data.value<Sensor*>();

    return (!m_replaceFilterEnabled || !sensor->isReplaced()) &&
           (!m_activeFilterEnabled  || sensor->isActive());
}
