#include "firesensordetector.h"

#include <QUdpSocket>

FireSensorDetector::FireSensorDetector(QObject *parent) : QObject{parent} {}

void FireSensorDetector::discoverSensors()
{
    qDebug() << "Discovering sensors...";
    QUdpSocket udpSocket;
    QByteArray datagram = "DiscoverFireSensors";
    udpSocket.writeDatagram(datagram, QHostAddress::Broadcast, 56000);
    qDebug() << "Broadcast message sent!";
}
