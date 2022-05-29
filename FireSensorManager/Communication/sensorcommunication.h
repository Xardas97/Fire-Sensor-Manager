#ifndef SENSORCOMMUNICATION_H
#define SENSORCOMMUNICATION_H

#include "communicationsensorstate.h"

#include <memory>
#include <vector>
#include <QObject>

class QHostAddress;
class FireSensorDetector;
class CommunicationSensorState;

class SensorCommunication : public QObject
{
    Q_OBJECT

private:
    static const int checkInactiveSensorsCount = 100;

public:
    SensorCommunication();
    ~SensorCommunication();

    bool updateData(Sensor& sensor);

    auto getKnownSensors() -> std::vector<std::shared_ptr<Sensor>>&;

    void discoverSensors();
    void discoverSensor(const QHostAddress& address, quint16 port);

private:
    void updateSensors();

    void onSensorDiscovered(std::shared_ptr<Sensor> sensor);

    std::vector<std::shared_ptr<Sensor>> knownSensors;
    std::unique_ptr<FireSensorDetector> fireSensorDetector;

    int sensorUpdatesCount = 0;
};

#endif // SENSORCOMMUNICATION_H
