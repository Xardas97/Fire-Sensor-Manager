#include "firesensordetector.h"

#include "ports.h"
#include "tcpserver.h"

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
    QByteArray datagram = "DiscoverFireSensors";

    for (int i = 0; i < Ports::maxSensorPorts; ++i)
    {
        // Sending broadcast to multiple ports to support simulating multiple sensors on same machine
        udpSocket.writeDatagram(datagram, QHostAddress::Broadcast, Ports::baseSensorPort + i);
    }

    qDebug() << "Broadcast message sent!";
}

void FireSensorDetector::onReceivedCommand(QTcpSocket* socket, QByteArray data)
{
    QString dataString = data;

    if (data.startsWith("DiscoveryReply"))
    {
        auto info = dataString.remove(0, 16).split(';');

        if (info.length() != 2)
        {
            qWarning() << "Sensor replied to discovery with broken data!";
            socket->write("0");
            socket->write("BROKEN_DATA");
            return;
        }

        QHostAddress sensorAddress {info[0]};
        quint16 sensorPort = info[1].toUShort();
        emit onSensorDiscovered(sensorAddress, sensorPort);

        socket->write("1");
        socket->write("ACK");
        return;
    }

    socket->write("0");
    socket->write("COMMAND_NOT_RECOGNIZED");
    qDebug() << "Command unknown, error response returned!";
}

FireSensorDetector::~FireSensorDetector() = default;
