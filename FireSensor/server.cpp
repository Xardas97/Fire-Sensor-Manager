#include "server.h"

#include "ports.h"
#include "tcpserver.h"
#include "tcpmessages.h"
#include "sensorstate.h"
#include "detectionservice.h"

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

    Capabilities capabilities = Capability::Temperature;
    sensorState = std::make_shared<SensorState>(capabilities, tcpServer->getServerAddress(), tcpServer->getServerPort());
    emit sensorStateChanged();

    detectionService = std::make_unique<DetectionService>(sensorState);

    return true;
}

void Server::onReceivedCommand(const TcpSocket& socket, const QJsonObject& data)
{
    if (data == TcpMessages::Command::GetData)
    {
        qDebug() << "Client asked for sensor data, returing temperature: " << sensorState->getTemperature();

        auto response = TcpMessages::Response::Ack;
        response["data"] = sensorState->toDataJson();
        socket.write(response);
        qDebug() << "Sensor data written";

        return;
    }

    socket.write(TcpMessages::Response::CommandNotRecognized);
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

SensorState* Server::getSensorState() const
{
    return sensorState.get();
}

Server::~Server() = default;
