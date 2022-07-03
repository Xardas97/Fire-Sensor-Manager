#include "sensorlist.h"
#include "sensor.h"

#include <algorithm>

SensorList::SensorList(std::vector<std::shared_ptr<Sensor>>& sensors, QObject* parent)
    : QAbstractListModel(parent),
      m_sensors(sensors)
{
    for (auto& sensor: m_sensors)
    {
        connectSignals(sensor);
    }
}

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

    connectSignals(sensor);

    if (sensor->alarmOn())
        emit sensorAlarmChanged(sensor.get());

    if (sensor->status() > 0)
        emit sensorStatusChanged(sensor.get());

    if (sensor->map())
        emit sensorPlacementChanged(sensor.get());
}

std::shared_ptr<Sensor> SensorList::remove(const Sensor& removeSensor)
{
    auto found = std::find_if(m_sensors.cbegin(),
                              m_sensors.cend(),
                              [&removeSensor](const std::shared_ptr<Sensor> sensor) { return sensor->uuid() == removeSensor.uuid(); });

    if (found == m_sensors.cend())
        return nullptr;

    // Holding it in a property temporarily because
    // ListView does not disconnect delegate immediatly
    m_removedSensor = *found;

    disconnectSignals(m_removedSensor);

    if (m_removedSensor->alarmOn())
        emit sensorAlarmChanged(m_removedSensor.get());

    if (m_removedSensor->status() > 0)
        emit sensorStatusChanged(m_removedSensor.get());

    if (m_removedSensor->map())
        emit sensorPlacementChanged(m_removedSensor.get());

    auto row = found - m_sensors.cbegin();

    beginRemoveRows(QModelIndex(), row, row);
    m_sensors.erase(found);
    endRemoveRows();

    return m_removedSensor;
}

void SensorList::connectSignals(std::shared_ptr<Sensor> sensor)
{
    QObject::connect(sensor.get(), &Sensor::isReplacedChanged, this, &SensorList::onDataChanged);
    QObject::connect(sensor.get(), &Sensor::isActiveChanged,   this, &SensorList::onDataChanged);
    QObject::connect(sensor.get(), &Sensor::mapChanged,        this, &SensorList::onDataChanged);

    QObject::connect(sensor.get(), &Sensor::alarmStateChanged, this, &SensorList::onAlarmChanged);
    QObject::connect(sensor.get(), &Sensor::statusChanged,     this, &SensorList::onStatusChanged);
    QObject::connect(sensor.get(), &Sensor::mapChanged,        this, &SensorList::onPlacementChanged);
}

void SensorList::disconnectSignals(std::shared_ptr<Sensor> sensor)
{
    QObject::disconnect(sensor.get(), &Sensor::isReplacedChanged, this, &SensorList::onDataChanged);
    QObject::disconnect(sensor.get(), &Sensor::isActiveChanged,   this, &SensorList::onDataChanged);
    QObject::disconnect(sensor.get(), &Sensor::mapChanged,        this, &SensorList::onDataChanged);

    QObject::disconnect(sensor.get(), &Sensor::alarmStateChanged, this, &SensorList::onAlarmChanged);
    QObject::disconnect(sensor.get(), &Sensor::statusChanged,     this, &SensorList::onStatusChanged);
    QObject::disconnect(sensor.get(), &Sensor::mapChanged,        this, &SensorList::onPlacementChanged);
}

void SensorList::onPlacementChanged()
{
    emit sensorPlacementChanged(static_cast<Sensor*>(QObject::sender()));
}

void SensorList::onAlarmChanged()
{
    emit sensorAlarmChanged(static_cast<Sensor*>(QObject::sender()));
}

void SensorList::onStatusChanged()
{
    emit sensorStatusChanged(static_cast<Sensor*>(QObject::sender()));
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
