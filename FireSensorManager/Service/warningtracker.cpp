#include "warningtracker.h"

#include "Map/mapentry.h"
#include "Map/floormaps.h"
#include "Communication/sensorlist.h"

WarningTracker::WarningTracker(std::shared_ptr<FloorMaps> floorMaps, std::shared_ptr<SensorList> knownSensors, QObject *parent)
    : QObject{parent},
      m_floorMaps{floorMaps},
      m_knownSensors{knownSensors}
{
    QObject::connect(m_knownSensors.get(), &SensorList::sensorAlarmChanged, this, &WarningTracker::onSensorAlarmChanged);
    QObject::connect(m_knownSensors.get(), &SensorList::sensorStatusChanged, this, &WarningTracker::onSensorStatusChanged);
    QObject::connect(m_knownSensors.get(), &SensorList::sensorPlacementChanged, this, &WarningTracker::onSensorPlacementChanged);
}

QList<Sensor*> WarningTracker::alarmedSensors()
{
    QList<Sensor*> alarmedSensors;

    for (auto& sensor: m_knownSensors->sensors())
    {
        if (sensor->alarmOn() && sensor->map())
            alarmedSensors.append(sensor.get());
    }

    return alarmedSensors;
}

QList<Sensor*> WarningTracker::warningSensors()
{
    QList<Sensor*> warningSensors;

    for (auto& sensor: m_knownSensors->sensors())
    {
        if (sensor->status() > 0 && sensor->map())
            warningSensors.append(sensor.get());
    }

    return warningSensors;
}

bool WarningTracker::isAlarmed(QVariant floor)
{
    if (floor.isNull())
        return false;

    auto isSensorOnFloor = [](std::shared_ptr<Sensor> sensor, int floor)
    {
        if (!sensor->map())
            return false;

        return floor == sensor->map()->floor();
    };

    for (auto& sensor: m_knownSensors->sensors())
    {
        if (sensor->alarmOn() && isSensorOnFloor(sensor, floor.toInt()))
            return true;
    }

    return false;
}

bool WarningTracker::isAlarmed(QVariant floor, QVariant floorPart)
{
    if (floor.isNull() || floorPart.isNull())
        return false;

    auto isSensorOnMap = [this](std::shared_ptr<Sensor> sensor, int floor, int floorPart)
    {
        int realFloor;
        short realFloorPart;
        auto found = m_floorMaps->getSensorLocation(sensor, realFloor, realFloorPart);
        if (!found)
            return false;

        return floor == realFloor && floorPart == realFloorPart;
    };

    for (auto& sensor: m_knownSensors->sensors())
    {
        if (sensor->alarmOn() && isSensorOnMap(sensor, floor.toInt(), floorPart.toInt()))
            return true;
    }

    return false;
}

void WarningTracker::onSensorAlarmChanged(Sensor* sensor)
{
    if (sensor->map())
        emit alarmedSensorsChanged();
}

void WarningTracker::onSensorStatusChanged(Sensor* sensor)
{
    if (sensor->map())
        emit warningSensorsChanged();
}

void WarningTracker::onSensorPlacementChanged(Sensor* sensor)
{
    if (sensor->alarmOn())
        emit alarmedSensorsChanged();

    if (sensor->status())
        emit warningSensorsChanged();
}
