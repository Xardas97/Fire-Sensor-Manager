#include "tcpserver.h"

#include "tcpmessages.h"

#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>

TcpServer::TcpServer(QObject *parent)
    : QObject{parent}, tcpServer(new QTcpServer())
{

    QObject::connect(tcpServer.get(), &QTcpServer::newConnection, this, &TcpServer::serverNewConnection);
}

bool TcpServer::startServer(const QHostAddress& address, quint16 port)
{
    qDebug() << "Starting TCP server...";

    bool success = tcpServer->listen(address, port);
    if (!success)
    {
        qWarning() << "TCP Server failed to start listening!";
        return false;
    }

    qDebug() << "TcpServer - Listening on port: " << port;
    return true;
}

void TcpServer::serverNewConnection()
{
    qDebug() << "TcpServer - New connection arrived";

    std::unique_ptr<QTcpSocket> socket(tcpServer->nextPendingConnection());

    qDebug() << "TcpServer - Waiting to read data";
    if (!socket->waitForReadyRead(3000))
    {
        qWarning() << "TcpServer - No data arrived!";
        return;
    }

    auto data = socket->readAll();
    qDebug() << "TcpServer - Data arrived: " << data;

    QJsonDocument dataDoc = QJsonDocument::fromJson(data);
    emit onReceivedCommand(TcpSocket(std::move(socket)), dataDoc.object());
}

bool TcpServer::isListening() const
{
    return tcpServer->isListening();
}

quint16 TcpServer::getServerPort() const
{
    return tcpServer->serverPort();
}

QHostAddress TcpServer::getServerAddress() const
{
    return tcpServer->serverAddress();
}

TcpServer::~TcpServer() = default;

TcpSocket::TcpSocket(std::unique_ptr<QTcpSocket> tcpSocket)
    : tcpSocket(std::move(tcpSocket))
{ }

void TcpSocket::write(const QJsonObject &responseData) const
{
    tcpSocket->write(TcpMessages::getBytes(responseData));
    tcpSocket->waitForBytesWritten();
}

TcpSocket::~TcpSocket() = default;
