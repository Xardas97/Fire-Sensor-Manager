#ifndef SENSORCOMMUNICATION_H
#define SENSORCOMMUNICATION_H

#include "sensorlist.h"

#include <memory>
#include <vector>
#include <QObject>

class Sensor;
class Database;
class QHostAddress;
class SensorDetector;

class SensorCommunication : public QObject
{
    Q_OBJECT

private:
    static const int check_inactive_sensors_count = 12;

public:
    SensorCommunication(std::shared_ptr<Database> database);
    ~SensorCommunication();

    bool updateData(Sensor& sensor);
    bool updateName(Sensor& sensor, const QString& name);

    SensorList& knownSensors();

    void removeSensor(Sensor& sensor);
    bool reactivateSensor(Sensor &sensor);

    void discoverSensors();
    void discoverSensor(const QHostAddress& address, quint16 port);

private:
    void updateSensors();

    void onSensorDiscovered(std::shared_ptr<Sensor> sensor);

    SensorList m_knownSensors;
    std::shared_ptr<Database>       m_database;
    std::unique_ptr<SensorDetector> m_sensorDetector;

    int m_sensorUpdatesCount = 0;
};

#endif // SENSORCOMMUNICATION_H
