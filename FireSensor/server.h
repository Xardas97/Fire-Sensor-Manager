#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <memory>

class QTcpServer;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    void startServer(int startingNumber = 1);

private slots:
    void serverNewConnection();

private:
    int nextNumber = 0;
    std::unique_ptr<QTcpServer> tcpServer;
};

#endif // SERVER_H
