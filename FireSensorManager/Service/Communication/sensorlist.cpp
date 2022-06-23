#include "sensorlist.h"
#include "sensor.h"

#include <algorithm>

SensorList::SensorList(std::vector<std::shared_ptr<Sensor>>& sensors, QObject* parent)
    : QAbstractListModel(parent),
      m_sensors(sensors)
{ }

std::shared_ptr<Sensor> SensorList::find(const Sensor& checkSensor) const
{
    auto found = std::find_if(m_sensors.cbegin(),
                              m_sensors.cend(),
                              [&checkSensor](const std::shared_ptr<Sensor> sensor) { return sensor->uuid() == checkSensor.uuid(); });

    if (found == m_sensors.cend())
        return nullptr;

    return *found;
}

void SensorList::add(std::shared_ptr<Sensor> sensor)
{
    beginInsertRows(QModelIndex(), m_sensors.size(), m_sensors.size());
    m_sensors.push_back(sensor);
    endInsertRows();

    QObject::connect(sensor.get(), &Sensor::isReplacedChanged, this, &SensorList::onDataChanged);
    QObject::connect(sensor.get(), &Sensor::isActiveChanged, this, &SensorList::onDataChanged);
}

std::shared_ptr<Sensor> SensorList::remove(const Sensor& removeSensor)
{
    auto found = std::find_if(m_sensors.cbegin(),
                              m_sensors.cend(),
                              [&removeSensor](const std::shared_ptr<Sensor> sensor) { return sensor->uuid() == removeSensor.uuid(); });

    if (found == m_sensors.cend())
        return nullptr;

    auto row = found - m_sensors.cbegin();

    QObject::disconnect(found->get(), &Sensor::isReplacedChanged, this, &SensorList::onDataChanged);
    QObject::disconnect(found->get(), &Sensor::isActiveChanged, this, &SensorList::onDataChanged);

    // Holding it in a property temporarily because
    // ListView does not disconnect delegate immediatly
    m_removedSensor = *found;

    beginRemoveRows(QModelIndex(), row, row);
    m_sensors.erase(found);
    endRemoveRows();

    return m_removedSensor;
}

void SensorList::onDataChanged()
{
    emit dataChanged(index(0), index(m_sensors.size() - 1));
}

QVariant SensorList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != DataRole)
        return QVariant();

    Sensor* sensor = m_sensors[index.row()].get();
    return QVariant::fromValue(sensor);
}

QHash<int, QByteArray> SensorList::roleNames() const
{
    static QHash<int, QByteArray> mapping {{DataRole, "data"}};
    return mapping;
}

int SensorList::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_sensors.size();
}

std::vector<std::shared_ptr<Sensor>>& SensorList::sensors()
{
    return m_sensors;
}
