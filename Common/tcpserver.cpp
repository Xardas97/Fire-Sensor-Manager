#include "tcpserver.h"

#include "tcpmessages.h"

#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>

TcpServer::TcpServer(QObject *parent)
    : QObject{parent}, m_tcpServer(new QTcpServer())
{

    QObject::connect(m_tcpServer.get(), &QTcpServer::newConnection, this, &TcpServer::serverNewConnection);
}

bool TcpServer::startServer(const QHostAddress& address, quint16 port)
{
    qDebug() << "Starting TCP server...";

    bool success = m_tcpServer->listen(address, port);
    if (!success)
    {
        qWarning() << "TCP Server failed to start listening!";
        return false;
    }

    qDebug() << "TcpServer - Listening on port: " << port;
    return true;
}

void TcpServer::stopServer()
{
    qDebug() << "Stopping TCP server";
    m_tcpServer->close();
}

void TcpServer::serverNewConnection()
{
    std::unique_ptr<QTcpSocket> socket(m_tcpServer->nextPendingConnection());

    if (!socket->waitForReadyRead(3000))
        return;

    auto data = socket->readAll();

    QJsonDocument dataDoc = QJsonDocument::fromJson(data);
    emit onReceivedCommand(TcpSocket(std::move(socket)), dataDoc.object());
}

bool TcpServer::isListening() const
{
    return m_tcpServer->isListening();
}

quint16 TcpServer::serverPort() const
{
    return m_tcpServer->serverPort();
}

QHostAddress TcpServer::serverAddress() const
{
    return m_tcpServer->serverAddress();
}

TcpServer::~TcpServer() = default;

TcpSocket::TcpSocket(std::unique_ptr<QTcpSocket> tcpSocket)
    : m_tcpSocket(std::move(tcpSocket))
{ }

void TcpSocket::write(const QJsonObject &responseData) const
{
    m_tcpSocket->write(TcpMessages::getBytes(responseData));
    m_tcpSocket->waitForBytesWritten();
}

TcpSocket::~TcpSocket() = default;
