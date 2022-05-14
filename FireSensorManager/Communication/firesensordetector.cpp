#include "firesensordetector.h"

#include "ports.h"
#include "tcpserver.h"
#include "tcpmessages.h"

#include <QUdpSocket>
#include <QTcpSocket>

FireSensorDetector::FireSensorDetector(QObject *parent)
    : QObject{parent}, tcpServer(new TcpServer())
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

void FireSensorDetector::onReceivedCommand(const TcpSocket& socket, const QJsonObject& data)
{
    if (data["command"] == TcpMessages::Command::Identify["command"])
    {
        if (!data.contains("address") || !data.contains("port"))
        {
            qWarning() << "Sensor did not reply with required data";
            socket.write(TcpMessages::Response::BrokenData);
            return;
        }

        QHostAddress sensorAddress {data["address"].toString()};
        quint16 sensorPort = data["port"].toInt();
        emit onSensorDiscovered(sensorAddress, sensorPort);

        socket.write(TcpMessages::Response::Ack);
        return;
    }

    socket.write(TcpMessages::Response::CommandNotRecognized);
    qDebug() << "Command unknown, error response returned!";
}

FireSensorDetector::~FireSensorDetector() = default;
