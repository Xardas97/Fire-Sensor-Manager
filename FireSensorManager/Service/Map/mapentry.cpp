#include "mapentry.h"
#include "../Communication/sensor.h"

MapEntry::MapEntry(int id, QPixmap pixmap)
    : m_id(id),
      m_pixmap(pixmap)
{ }

void MapEntry::addSensor(std::shared_ptr<Sensor> sensor)
{
    auto oldMap = sensor->map();
    if (oldMap)
        oldMap->removeSensor(sensor);

    m_placedSensors.push_back(sensor);
    sensor->setMap(this);
}

void MapEntry::removeSensor(std::shared_ptr<Sensor> sensor)
{
    if (m_placedSensors.empty())
        return;

    auto sensorIt = std::find(m_placedSensors.cbegin(), m_placedSensors.cend(), sensor);
    if (sensorIt != m_placedSensors.cend())
    {
        sensor->setMap(nullptr);
        m_placedSensors.erase(sensorIt);
    }
}

int MapEntry::id() const
{
    return m_id;
}

void MapEntry::setId(int id)
{
    m_id = id;
}

QPixmap& MapEntry::pixmap()
{
    return m_pixmap;
}

const QPixmap& MapEntry::pixmap() const
{
    return m_pixmap;
}

const std::vector<std::shared_ptr<Sensor>>& MapEntry::placedSensors() const
{
    return m_placedSensors;
}
