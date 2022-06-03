#include "sensorlist.h"

SensorList::SensorList(QObject* parent)
    : QAbstractListModel(parent)
{ }

std::shared_ptr<Sensor> SensorList::find(std::shared_ptr<Sensor> checkSensor) const
{
    auto found = std::find_if(sensors.cbegin(),
                              sensors.cend(),
                              [&checkSensor](const std::shared_ptr<Sensor> sensor) { return sensor->getUuid() == checkSensor->getUuid(); });

    if (found == sensors.cend())
        return nullptr;

    return *found;
}

void SensorList::add(std::shared_ptr<Sensor> sensor)
{
    beginInsertRows(QModelIndex(), sensors.size(), sensors.size());
    sensors.push_back(sensor);
    endInsertRows();
}

void SensorList::remove(std::shared_ptr<Sensor> removeSensor)
{
    auto found = std::find_if(sensors.cbegin(),
                              sensors.cend(),
                              [&removeSensor](const std::shared_ptr<Sensor> sensor) { return sensor->getUuid() == removeSensor->getUuid(); });

    if (found == sensors.cend())
        return;

    auto row = found - sensors.cbegin();

    beginRemoveRows(QModelIndex(), row, row);
    sensors.erase(found);
    endRemoveRows();
}

QVariant SensorList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != DataRole)
        return QVariant();

    Sensor* sensor = sensors[index.row()].get();
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

    return sensors.size();
}

std::vector<std::shared_ptr<Sensor>>& SensorList::getSensors()
{
    return sensors;
}
