#ifndef SERVICE_H
#define SERVICE_H

#include <memory>
#include <QObject>

class SensorState;
class QHostAddress;
class FireSensorDetector;

class Service : public QObject
{
    Q_OBJECT

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    int getTemperature();
    void discoverSensors();

private:
    void onSensorDiscovered(std::shared_ptr<SensorState> sensor);

    std::unique_ptr<FireSensorDetector> fireSensorDetector;
};

#endif // SERVICE_H
