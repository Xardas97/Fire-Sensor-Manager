#ifndef SERVICE_H
#define SERVICE_H

#include <memory>
#include <QObject>

class SensorCommunication;

class Service : public QObject
{
    Q_OBJECT

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    int getTemperature();
    void discoverSensor(const QString& address, quint16 port);
    void discoverSensors();

private:
    std::unique_ptr<SensorCommunication> sensorCommunication;
};

#endif // SERVICE_H
