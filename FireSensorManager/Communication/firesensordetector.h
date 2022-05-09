#ifndef FIRESENSORDETECTOR_H
#define FIRESENSORDETECTOR_H

#include <memory>
#include <QObject>

class TcpServer;
class QTcpSocket;
class QHostAddress;

class FireSensorDetector : public QObject
{
    Q_OBJECT
public:
    explicit FireSensorDetector(QObject *parent = nullptr);
    ~FireSensorDetector();

    void discoverSensors();

signals:
    void onSensorDiscovered(const QHostAddress& address, quint16 port);

private slots:
    void onReceivedCommand(QTcpSocket* socket, QByteArray data);

private:
    std::unique_ptr<TcpServer> tcpServer;
};

#endif // FIRESENSORDETECTOR_H
