#include "firesensordetector.h"

#include "ports.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "tcpmessages.h"

#include <QUdpSocket>
#include <QTcpSocket>

FireSensorDetector::FireSensorDetector()
    : tcpServer(new TcpServer())
{
    QObject::connect(tcpServer.get(), &TcpServer::onReceivedCommand, this, &FireSensorDetector::onReceivedCommand);
}

void FireSensorDetector::discoverSensors()
{
    qDebug() << "Discovering sensors...";

    if (!tcpServer->isListening())
    {
        qDebug() << "Starting discovering TcpServer...";
        bool started = tcpServer->startServer(QHostAddress::Any, Ports::sensorDetectorPort);
        if (!started) return;
    }

    QUdpSocket udpSocket;

    for (int i = 0; i < Ports::maxSensorPorts; ++i)
    {
        // Sending broadcast to multiple ports to support simulating multiple sensors on same machine
        auto command = TcpMessages::getBytes(TcpMessages::Command::DiscoverSensors);
        udpSocket.writeDatagram(command, QHostAddress::Broadcast, Ports::baseSensorPort + i);
    }

    qDebug() << "Broadcast message sent!";
}

bool FireSensorDetector::discoverSensor(const QHostAddress& address, quint16 port)
{
    TcpClient tcpClient;
    auto response = tcpClient.sendRequest(address, port, TcpMessages::Command::Identify);
    return parseIncomingSensorIdentification(response);
}

void FireSensorDetector::onReceivedCommand(const TcpSocket& socket, const QJsonObject& command)
{
    if (command["command"] == TcpMessages::Command::Identify["command"])
    {
        auto success = parseIncomingSensorIdentification(command);
        if (success)
        {
            socket.write(TcpMessages::Response::Ack);
        }
        else
        {
            qWarning() << "Sensor returned no data!";
            socket.write(TcpMessages::Response::BrokenData);
        }

        return;
    }

    socket.write(TcpMessages::Response::CommandNotRecognized);
    qDebug() << "Command unknown, error response returned!";
}

bool FireSensorDetector::parseIncomingSensorIdentification(const QJsonObject& json)
{
    if (!json.contains("data"))
    {
        qWarning() << "Sensor returned no data!";
        return false;
    }

    auto sensor = Sensor::fromJson(json["data"].toObject());
    emit onSensorDiscovered(std::move(sensor));
    return true;
}

FireSensorDetector::~FireSensorDetector() = default;
