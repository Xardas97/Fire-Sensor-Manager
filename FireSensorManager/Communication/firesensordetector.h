#ifndef FIRESENSORDETECTOR_H
#define FIRESENSORDETECTOR_H

#include <memory>
#include <QObject>

class TcpServer;
class TcpSocket;
class SensorState;
class QHostAddress;

class FireSensorDetector : public QObject
{
    Q_OBJECT
public:
    explicit FireSensorDetector(QObject *parent = nullptr);
    ~FireSensorDetector();

    void discoverSensors();

signals:
    void onSensorDiscovered(std::shared_ptr<SensorState> sensorState);

private:
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);

    std::unique_ptr<TcpServer> tcpServer;
};

#endif // FIRESENSORDETECTOR_H
