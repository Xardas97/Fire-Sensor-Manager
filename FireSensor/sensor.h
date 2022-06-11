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

    Q_PROPERTY(SensorState* sensorState READ sensorState NOTIFY sensorStateChanged)
public:
    explicit Sensor(QObject *parent = nullptr);
    ~Sensor();

    SensorState* sensorState() const;

public slots:
    bool startSensor(Capabilities capabilities);
    void stopSensor();

    bool loadSensor(const QUrl& url);
    bool saveSensor(const QUrl& url);

signals:
    void sensorStateChanged();

private:
    bool startServer();
    QHostAddress getLocalAddress();
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);

    std::shared_ptr<SensorState> m_sensorState;

    std::unique_ptr<TcpServer> m_tcpServer;
    std::unique_ptr<DetectionService> m_detectionService;
};

#endif // SENSOR_H
