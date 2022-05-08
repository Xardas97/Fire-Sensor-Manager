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
    void startServer(const QHostAddress& address, quint16 port);

signals:
    void onReceivedCommand(QTcpSocket* socket, QByteArray data);

private slots:
    void serverNewConnection();

private:
    std::unique_ptr<QTcpServer> tcpServer;
};

#endif // TCPSERVER_H
