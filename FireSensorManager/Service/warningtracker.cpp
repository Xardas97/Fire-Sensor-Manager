#include "warningtracker.h"

#include "Communication/sensorlist.h"

WarningTracker::WarningTracker(std::shared_ptr<SensorList> knownSensors, QObject *parent)
    : QObject{parent},
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
