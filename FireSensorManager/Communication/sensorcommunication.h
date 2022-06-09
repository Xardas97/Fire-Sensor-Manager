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
    static const int check_inactive_sensors_count = 5;

public:
    SensorCommunication();
    ~SensorCommunication();

    bool updateData(Sensor& sensor);
    bool updateName(Sensor& sensor, const QString& name);

    SensorList& knownSensors();

    void discoverSensors();
    void discoverSensor(const QHostAddress& address, quint16 port);

private:
    void updateSensors();

    void onSensorDiscovered(std::shared_ptr<Sensor> sensor);

    SensorList m_knownSensors;
    std::unique_ptr<SensorDetector> m_sensorDetector;

    int m_sensorUpdatesCount = 0;
};

#endif // SENSORCOMMUNICATION_H
