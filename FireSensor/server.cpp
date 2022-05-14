#include "server.h"

#include "ports.h"
#include "tcpserver.h"
#include "tcpmessages.h"

#include <QDebug>
#include <QTcpSocket>
#include <QNetworkInterface>

Server::Server(QObject *parent)
    : QObject{parent}, tcpServer(new TcpServer())
{
    QObject::connect(tcpServer.get(), &TcpServer::onReceivedCommand, this, &Server::onReceivedCommand);
}

bool Server::startServer()
{
    auto localAddress = getLocalAddress();

    for (int i = 0; i < Ports::maxSensorPorts; ++i)
    {
        auto started = tcpServer->startServer(localAddress, Ports::baseSensorPort + i);
        if (started) break;
    }

    if (!tcpServer->isListening())
    {
        qWarning() << "Failed to start the server on any of the ports!";
        return false;
    }

    return true;
}

void Server::onReceivedCommand(QTcpSocket* socket, QByteArray data)
{
    if (data == TcpMessages::Command::GetNumber)
    {
        auto currentNumber = nextNumber++;
        emit nextNumberChanged();
        qDebug() << "Client asked for next number, returing: " << currentNumber;

        socket->write(std::to_string(currentNumber).c_str());
        qDebug() << "Number data written";

        return;
    }

    socket->write(TcpMessages::Reply::CommandNotRecognized);
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

void Server::setNextNumber(int nextNumber)
{
    qDebug() << "User set nextNumber to " << nextNumber;
    this->nextNumber = nextNumber;
    emit nextNumberChanged();
}

int Server::getNextNumber()
{
    return this->nextNumber;
}

Server::~Server() = default;
