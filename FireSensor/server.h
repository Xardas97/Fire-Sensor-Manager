#ifndef SERVER_H
#define SERVER_H

#include "sensorstate.h"

#include <QObject>
#include <memory>

class TcpServer;
class TcpSocket;
class QHostAddress;
class DetectionService;

class Server : public QObject
{
    Q_OBJECT

    Q_PROPERTY(SensorState* sensorState READ getSensorState NOTIFY sensorStateChanged)
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    bool startServer();
    SensorState* getSensorState() const;

signals:
    void sensorStateChanged();

private:
    QHostAddress getLocalAddress();
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);

    std::shared_ptr<SensorState> sensorState;

    std::unique_ptr<TcpServer> tcpServer;
    std::unique_ptr<DetectionService> detectionService;
};

#endif // SERVER_H
