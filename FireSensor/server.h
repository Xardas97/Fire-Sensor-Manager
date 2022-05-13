#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <memory>

class TcpServer;
class QTcpSocket;
class QHostAddress;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    bool startServer(int startingNumber = 1);

    quint16 getServerPort() const;
    QHostAddress getServerAddress() const;

private:
    QHostAddress getLocalAddress();
    void onReceivedCommand(QTcpSocket*, QByteArray);

    int nextNumber = 0;
    std::unique_ptr<TcpServer> tcpServer;
};

#endif // SERVER_H
