#ifndef SENSORCOMMUNICATION_H
#define SENSORCOMMUNICATION_H

#include <memory>
#include <vector>
#include <QObject>

class SensorState;
class QHostAddress;
class FireSensorDetector;

class SensorCommunication : public QObject
{
    Q_OBJECT
public:
    SensorCommunication();
    ~SensorCommunication();

    bool updateData(SensorState& sensor);

    auto getKnownSensors() -> std::vector<std::shared_ptr<SensorState>>&;

    void discoverSensors();
    void discoverSensor(const QHostAddress& address, quint16 port);

private:
    void onSensorDiscovered(std::shared_ptr<SensorState> sensor);

    std::vector<std::shared_ptr<SensorState>> knownSensors;
    std::unique_ptr<FireSensorDetector> fireSensorDetector;
};

#endif // SENSORCOMMUNICATION_H
