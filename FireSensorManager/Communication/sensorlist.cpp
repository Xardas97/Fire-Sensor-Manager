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

    QObject::connect(sensor.get(), &Sensor::isReplacedChanged, this, &SensorList::onDataChanged);
    QObject::connect(sensor.get(), &Sensor::isActiveChanged, this, &SensorList::onDataChanged);
}

void SensorList::remove(std::shared_ptr<Sensor> removeSensor)
{
    auto found = std::find_if(sensors.cbegin(),
                              sensors.cend(),
                              [&removeSensor](const std::shared_ptr<Sensor> sensor) { return sensor->getUuid() == removeSensor->getUuid(); });

    if (found == sensors.cend())
        return;

    auto row = found - sensors.cbegin();

    QObject::disconnect(found->get(), &Sensor::isReplacedChanged, this, &SensorList::onDataChanged);
    QObject::disconnect(found->get(), &Sensor::isActiveChanged, this, &SensorList::onDataChanged);

    beginRemoveRows(QModelIndex(), row, row);
    sensors.erase(found);
    endRemoveRows();
}

void SensorList::onDataChanged()
{
    emit dataChanged(index(0), index(sensors.size() - 1));
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
