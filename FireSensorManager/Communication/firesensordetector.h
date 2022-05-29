#ifndef FIRESENSORDETECTOR_H
#define FIRESENSORDETECTOR_H

#include "communicationsensorstate.h"

#include <memory>
#include <QObject>

class TcpServer;
class TcpSocket;
class QHostAddress;

class FireSensorDetector : public QObject
{
    Q_OBJECT
public:
    explicit FireSensorDetector();
    ~FireSensorDetector();

    void discoverSensors();
    bool discoverSensor(const QHostAddress& address, quint16 port);

signals:
    void onSensorDiscovered(std::shared_ptr<Sensor> sensorState);

private:
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);
    bool parseIncomingSensorIdentification(const QJsonObject& json);

    std::unique_ptr<TcpServer> tcpServer;
};

#endif // FIRESENSORDETECTOR_H
