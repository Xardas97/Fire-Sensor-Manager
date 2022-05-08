#include "server.h"
#include "tcpserver.h"

#include <QDebug>
#include <QTcpSocket>

Server::Server(QObject *parent)
    : QObject{parent}, tcpServer(new TcpServer())
{
    QObject::connect(tcpServer.get(), &TcpServer::onReceivedCommand, this, &Server::onReceivedCommand);
}

void Server::startServer(int startingNumber)
{
    tcpServer->startServer(QHostAddress::Any, 56000);
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

Server::~Server() = default;
