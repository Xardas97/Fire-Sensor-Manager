#ifndef WARNINGTRACKER_H
#define WARNINGTRACKER_H

#include "Communication/sensor.h"

#include <memory>
#include <QObject>

class SensorList;

class WarningTracker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Sensor*> alarmedSensors READ alarmedSensors NOTIFY alarmedSensorsChanged)
    Q_PROPERTY(QList<Sensor*> warningSensors READ warningSensors NOTIFY warningSensorsChanged)
public:
    explicit WarningTracker(std::shared_ptr<SensorList> knownSensors, QObject *parent = nullptr);

    QList<Sensor*> alarmedSensors();
    QList<Sensor*> warningSensors();

signals:
    void alarmedSensorsChanged();
    void warningSensorsChanged();

private:
    void onSensorAlarmChanged(Sensor* sensor);
    void onSensorStatusChanged(Sensor* sensor);
    void onSensorPlacementChanged(Sensor* sensor);

    std::shared_ptr<SensorList> m_knownSensors;
};

#endif // WARNINGTRACKER_H
