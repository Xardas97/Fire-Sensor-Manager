#ifndef SENSORCOMMUNICATION_H
#define SENSORCOMMUNICATION_H

#include "sensorlist.h"

#include <memory>
#include <vector>
#include <QObject>

class Sensor;
class QHostAddress;
class SensorDetector;

class SensorCommunication : public QObject
{
    Q_OBJECT

private:
    static const int checkInactiveSensorsCount = 5;

public:
    SensorCommunication();
    ~SensorCommunication();

    bool updateData(Sensor& sensor);

    SensorList& getKnownSensors();

    void discoverSensors();
    void discoverSensor(const QHostAddress& address, quint16 port);

private:
    void updateSensors();

    void onSensorDiscovered(std::shared_ptr<Sensor> sensor);

    SensorList knownSensors;
    std::unique_ptr<SensorDetector> fireSensorDetector;

    int sensorUpdatesCount = 0;
};

#endif // SENSORCOMMUNICATION_H
