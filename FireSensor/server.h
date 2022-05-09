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

    void startServer(int startingNumber = 1);

    quint16 getServerPort() const;
    QHostAddress getServerAddress() const;

private slots:
    void onReceivedCommand(QTcpSocket*, QByteArray);

private:
    QHostAddress getLocalAddress();

    int nextNumber = 0;
    std::unique_ptr<TcpServer> tcpServer;
};

#endif // SERVER_H
