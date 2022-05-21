#ifndef SENSOR_H
#define SENSOR_H

#include "sensorstate.h"

#include <QObject>
#include <memory>

class TcpServer;
class TcpSocket;
class QHostAddress;
class DetectionService;

class Sensor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(SensorState* sensorState READ getSensorState NOTIFY sensorStateChanged)
public:
    explicit Sensor(QObject *parent = nullptr);
    ~Sensor();

    SensorState* getSensorState() const;

public slots:
    bool startSensor(Capabilities capabilities);

signals:
    void sensorStateChanged();

private:
    QHostAddress getLocalAddress();
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);

    std::shared_ptr<SensorState> sensorState;

    std::unique_ptr<TcpServer> tcpServer;
    std::unique_ptr<DetectionService> detectionService;
};

#endif // SENSOR_H
