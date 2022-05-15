#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <memory>

class TcpServer;
class TcpSocket;
class SensorState;
class QHostAddress;

class Server : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int temperature READ getTemperature WRITE setTemperature NOTIFY temperatureChanged)
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    bool startServer();

    quint16 getServerPort() const;
    QHostAddress getServerAddress() const;

public slots:
    void setTemperature(int temperature);
    int getTemperature();

signals:
    void temperatureChanged();

private:
    QHostAddress getLocalAddress();
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);

    std::unique_ptr<SensorState> sensorState;
    std::unique_ptr<TcpServer> tcpServer;
};

#endif // SERVER_H
