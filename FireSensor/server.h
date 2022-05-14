#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <memory>

class TcpServer;
class TcpSocket;
class QHostAddress;

class Server : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int nextNumber READ getNextNumber WRITE setNextNumber NOTIFY nextNumberChanged)
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    bool startServer();

    quint16 getServerPort() const;
    QHostAddress getServerAddress() const;

public slots:
    void setNextNumber(int nextNumber);
    int getNextNumber();

signals:
    void nextNumberChanged();

private:
    QHostAddress getLocalAddress();
    void onReceivedCommand(const TcpSocket& socket, const QJsonObject& data);

    int nextNumber = 0;
    std::unique_ptr<TcpServer> tcpServer;
};

#endif // SERVER_H
