#include "tcpserver.h"

#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

TcpServer::TcpServer(QObject *parent)
    : QObject{parent}, tcpServer(new QTcpServer())
{

}

bool TcpServer::startServer(const QHostAddress& address, quint16 port)
{
    qDebug() << "Starting TCP server...";

    QObject::connect(tcpServer.get(), &QTcpServer::newConnection, this, &TcpServer::serverNewConnection);

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

    auto socket = tcpServer->nextPendingConnection();

    qDebug() << "TcpServer - Waiting to read data";
    if (!socket->waitForReadyRead(3000))
    {
        qWarning() << "TcpServer - No data arrived!";
        return;
    }

    auto data = socket->readAll();
    qDebug() << "TcpServer - Data arrived: " << data;

    emit onReceivedCommand(socket, data);
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
