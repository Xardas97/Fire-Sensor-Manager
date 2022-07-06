#include "filteredsensorlistmodel.h"

#include "../Communication/sensor.h"
#include "../Communication/sensorlist.h"

FilteredSensorListModel::FilteredSensorListModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool FilteredSensorListModel::activeFilterEnabled() const
{
    return m_activeFilterEnabled;
}

void FilteredSensorListModel::setactiveFilterEnabled(bool activeFilterEnabled)
{
    if (m_activeFilterEnabled == activeFilterEnabled)
        return;

    m_activeFilterEnabled = activeFilterEnabled;

    emit activeFilterEnabledChanged();
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

bool FilteredSensorListModel::alarmedFilterEnabled() const
{
    return m_alarmedFilterEnabled;
}

void FilteredSensorListModel::setAlarmedFilterEnabled(bool alarmedFilterEnabled)
{
    if (m_alarmedFilterEnabled == alarmedFilterEnabled)
        return;

    m_alarmedFilterEnabled = alarmedFilterEnabled;

    emit alarmedFilterEnabledChanged();
    invalidateFilter();
}

bool FilteredSensorListModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_activeFilterEnabled && !m_placedFilterEnabled && !m_unplacedFilterEnabled && !m_alarmedFilterEnabled)
        return true;

    const auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    const auto data = index.data(SensorList::Roles::DataRole);
    Sensor* sensor = data.value<Sensor*>();

    return !(m_placedFilterEnabled && !sensor->map()) &&
           !(m_unplacedFilterEnabled && sensor->map()) &&
           !(m_activeFilterEnabled && (!sensor->isActive() || sensor->isReplaced())) &&
           !(m_alarmedFilterEnabled && !sensor->alarmOn());
}
