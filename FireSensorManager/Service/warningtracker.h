#ifndef WARNINGTRACKER_H
#define WARNINGTRACKER_H

#include "Communication/sensor.h"

#include <memory>
#include <QObject>

class FloorMaps;
class SensorList;

class WarningTracker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Sensor*> alarmedSensors READ alarmedSensors NOTIFY alarmedSensorsChanged)
    Q_PROPERTY(QList<Sensor*> warningSensors READ warningSensors NOTIFY warningSensorsChanged)
public:
    explicit WarningTracker(std::shared_ptr<FloorMaps> floorMaps, std::shared_ptr<SensorList> knownSensors, QObject *parent = nullptr);

    QList<Sensor*> alarmedSensors();
    QList<Sensor*> warningSensors();

public slots:
    bool isAlarmed(QVariant floor);
    bool isAlarmed(QVariant floor, QVariant floorPart);

signals:
    void alarmedSensorsChanged();
    void warningSensorsChanged();

private:
    void onSensorAlarmChanged(Sensor* sensor);
    void onSensorStatusChanged(Sensor* sensor);
    void onSensorPlacementChanged(Sensor* sensor);

    std::shared_ptr<FloorMaps> m_floorMaps;
    std::shared_ptr<SensorList> m_knownSensors;
};

#endif // WARNINGTRACKER_H
