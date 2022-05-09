#include "firesensordetector.h"

#include <QUdpSocket>
#include <QTcpSocket>
#include "tcpserver.h"

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
        bool started = tcpServer->startServer(QHostAddress::Any, 56560);
        if (!started) return;
    }

    QUdpSocket udpSocket;
    QByteArray datagram = "DiscoverFireSensors";
    udpSocket.writeDatagram(datagram, QHostAddress::Broadcast, 56000);

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
