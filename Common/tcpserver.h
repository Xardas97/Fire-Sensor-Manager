#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QHostAddress>

#include <memory>

class QTcpSocket;
class QTcpServer;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    bool startServer(const QHostAddress& address, quint16 port);
    bool isListening() const;

    quint16 getServerPort() const;
    QHostAddress getServerAddress() const;

signals:
    void onReceivedCommand(QTcpSocket* socket, QByteArray data);

private slots:
    void serverNewConnection();

private:
    std::unique_ptr<QTcpServer> tcpServer;
};

#endif // TCPSERVER_H
