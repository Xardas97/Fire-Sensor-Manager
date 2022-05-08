#include "server.h"

#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

Server::Server(QObject *parent)
    : QObject{parent}, tcpServer(new QTcpServer())
{

}

void Server::startServer(int startingNumber)
{
    auto port = 56000;

    qDebug() << "Starting TCP server, starting number: " << startingNumber;
    nextNumber = startingNumber;

    QObject::connect(tcpServer.get(), &QTcpServer::newConnection, this, &Server::serverNewConnection);

    bool success = tcpServer->listen(QHostAddress::Any, port);
    if (!success)
    {
        qWarning() << "TCP Server failed to start listening!";
        return;
    }

    port = tcpServer->serverPort();
    qDebug() << "Listening on port: " << port;
}

void Server::serverNewConnection()
{
    qDebug() << "New connection arrived";

    auto socket = tcpServer->nextPendingConnection();

    qDebug() << "Waiting to read data";
    if (!socket->waitForReadyRead(3000))
    {
        qWarning() << "No data arrived!";
        return;
    }

    auto data = socket->readAll();
    qDebug() << "Data arrived: " << data;

    if (data == "GET_NUMBER")
    {
        socket->write("1");

        auto currentNumber = nextNumber++;
        qDebug() << "Client asked for next number, returing: " << currentNumber;

        socket->write(std::to_string(currentNumber).c_str());
        qDebug() << "Number data written";

        return;
    }

    socket->write("0");
    socket->write("COMMAND_NOT_RECOGNIZED");
    qDebug() << "Command unknown, error response returned!";
}

Server::~Server() = default;
