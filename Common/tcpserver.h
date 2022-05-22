#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QJsonObject>
#include <QHostAddress>

#include <memory>

class QTcpSocket;
class QTcpServer;

class TcpSocket
{
    public:
        TcpSocket(std::unique_ptr<QTcpSocket> tcpSocket);
        ~TcpSocket();

        void write(const QJsonObject& responseData) const;

    private:
        std::unique_ptr<QTcpSocket> tcpSocket;
};

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    bool startServer(const QHostAddress& address, quint16 port);
    void stopServer();
    bool isListening() const;

    quint16 getServerPort() const;
    QHostAddress getServerAddress() const;

signals:
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);

private:
    void serverNewConnection();

    std::unique_ptr<QTcpServer> tcpServer;
};

#endif // TCPSERVER_H
