#include "filteredsensorlistmodel.h"

#include "Communication/sensor.h"
#include "Communication/sensorlist.h"

FilteredSensorListModel::FilteredSensorListModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool FilteredSensorListModel::inactiveFilterEnabled() const
{
    return m_inactiveFilterEnabled;
}

void FilteredSensorListModel::setinActiveFilterEnabled(bool inactiveFilterEnabled)
{
    if (m_inactiveFilterEnabled == inactiveFilterEnabled)
        return;

    m_inactiveFilterEnabled = inactiveFilterEnabled;

    emit inactiveFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::placedFilterEnabled() const
{
    return m_placedFilterEnabled;
}

void FilteredSensorListModel::setPlacedFilterEnabled(bool placedFilterEnabled)
{
    if (m_placedFilterEnabled == placedFilterEnabled)
        return;

    m_placedFilterEnabled = placedFilterEnabled;

    emit placedFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::unplacedFilterEnabled() const
{
    return m_unplacedFilterEnabled;
}

void FilteredSensorListModel::setUnplacedFilterEnabled(bool unplacedFilterEnabled)
{
    if (m_unplacedFilterEnabled == unplacedFilterEnabled)
        return;

    m_unplacedFilterEnabled = unplacedFilterEnabled;

    emit unplacedFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_inactiveFilterEnabled && !m_placedFilterEnabled && !m_unplacedFilterEnabled)
        return true;

    const auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    const auto data = index.data(SensorList::Roles::DataRole);
    Sensor* sensor = data.value<Sensor*>();

    return !(m_placedFilterEnabled && sensor->map()) &&
           !(m_unplacedFilterEnabled && !sensor->map()) &&
           !(m_inactiveFilterEnabled && (!sensor->isActive() || sensor->isReplaced()));
}
