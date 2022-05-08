#include "detectionservice.h"

#include <QUdpSocket>
#include <QDebug>

DetectionService::DetectionService(QObject *parent)
    : QObject{parent}, udpSocket(new QUdpSocket())
{
    auto port = 56000;
    udpSocket->bind(port, QUdpSocket::ShareAddress);
    QObject::connect(udpSocket.get(), &QUdpSocket::readyRead, this, &DetectionService::processDetectionRequest);
    qDebug() << "DetectionService listening on port " << port;
}

void DetectionService::processDetectionRequest()
{
    qDebug() << "DetectionService received a request";

    QHostAddress peerAddress;
    quint16 peerPort;

    if (!udpSocket->hasPendingDatagrams())
    {
        qWarning() << "Udp request received with no pending data!";
        return;
    }

    QByteArray data;
    data.resize(int(udpSocket->pendingDatagramSize()));
    udpSocket->readDatagram(data.data(), data.size(), &peerAddress, &peerPort);
    qDebug() << "Received command: " << data;

    if (data == "DiscoverFireSensors")
    {
        qDebug() << "Replying with my IP and port to requester: " << peerAddress << ";  " << peerPort;
        return;
    }

    qDebug() << "Unknown command, ignoring...";
}

DetectionService::~DetectionService() = default;
