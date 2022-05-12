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
        udpSocket.writeDatagram(TcpMessages::Command::DiscoverSensors, QHostAddress::Broadcast, Ports::baseSensorPort + i);
    }

    qDebug() << "Broadcast message sent!";
}

void FireSensorDetector::onReceivedCommand(QTcpSocket* socket, QByteArray data)
{
    QString dataString = data;

    if (data.startsWith(TcpMessages::Command::Identify))
    {
        auto info = dataString.remove(0, 16).split(';');

        if (info.length() != 2)
        {
            qWarning() << "Sensor replied to discovery with broken data!";
            socket->write(TcpMessages::Reply::BrokenData);
            return;
        }

        QHostAddress sensorAddress {info[0]};
        quint16 sensorPort = info[1].toUShort();
        emit onSensorDiscovered(sensorAddress, sensorPort);

        socket->write(TcpMessages::Reply::Ack);
        return;
    }

    socket->write(TcpMessages::Reply::CommandNotRecognized);
    qDebug() << "Command unknown, error response returned!";
}

FireSensorDetector::~FireSensorDetector() = default;
