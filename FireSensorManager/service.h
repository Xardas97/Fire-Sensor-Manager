#ifndef SERVICE_H
#define SERVICE_H

#include <memory>
#include <QObject>

class CommunicationSensorState;

class SensorCommunication;

class Service : public QObject
{
    Q_OBJECT

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    void discoverSensor(const QString& address, quint16 port);
    void discoverSensors();

    CommunicationSensorState* getRandomSensor();

private:
    std::unique_ptr<SensorCommunication> sensorCommunication;
};

#endif // SERVICE_H
