#ifndef SENSORDETECTOR_H
#define SENSORDETECTOR_H

#include <memory>
#include <QObject>

class Sensor;
class TcpServer;
class TcpSocket;
class QHostAddress;

class SensorDetector : public QObject
{
    Q_OBJECT
public:
    explicit SensorDetector();
    ~SensorDetector();

    void discoverSensors();
    bool discoverSensor(const QHostAddress& address, quint16 port);

signals:
    void onSensorDiscovered(std::shared_ptr<Sensor> sensorState);

private:
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);
    bool parseIncomingSensorIdentification(const QJsonObject& json);

    std::unique_ptr<TcpServer> tcpServer;
};

#endif // SENSORDETECTOR_H
