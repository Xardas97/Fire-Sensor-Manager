#include "server.h"

#include "ports.h"
#include "tcpserver.h"

#include <QDebug>
#include <QTcpSocket>
#include <QNetworkInterface>

Server::Server(QObject *parent)
    : QObject{parent}, tcpServer(new TcpServer())
{
    QObject::connect(tcpServer.get(), &TcpServer::onReceivedCommand, this, &Server::onReceivedCommand);
}

void Server::startServer(int startingNumber)
{
    auto localAddress = getLocalAddress();
    tcpServer->startServer(localAddress, Ports::baseSensorPort);
    nextNumber = startingNumber;
}

void Server::onReceivedCommand(QTcpSocket* socket, QByteArray data)
{
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

QHostAddress Server::getLocalAddress()
{
    auto addresses = QNetworkInterface::allAddresses();
    for (const auto &address: addresses)
    {
        if (address.isLoopback())
            continue;

        if (address.protocol() != QAbstractSocket::NetworkLayerProtocol::IPv4Protocol)
            continue;

        return address;
    }

    return QHostAddress::Null;
}

quint16 Server::getServerPort() const
{
    return tcpServer->getServerPort();
}

QHostAddress Server::getServerAddress() const
{
    return tcpServer->getServerAddress();
}

Server::~Server() = default;
