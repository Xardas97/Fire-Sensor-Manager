#include "detectionservice.h"

#include <QDebug>
#include <QUdpSocket>
#include "tcpclient.h"

DetectionService::DetectionService(const QHostAddress& serverAddress, quint16 serverPort, QObject *parent)
    : QObject{parent}, serverAddress(serverAddress), serverPort(serverPort), udpSocket(new QUdpSocket())
{
    auto port = 56000;
    udpSocket->bind(port, QUdpSocket::ShareAddress);
    QObject::connect(udpSocket.get(), &QUdpSocket::readyRead, this, &DetectionService::processDetectionRequest);
    qDebug() << "DetectionService listening on port " << port;
}

void DetectionService::processDetectionRequest()
{
    qDebug() << "DetectionService received a request";

    if (!udpSocket->hasPendingDatagrams())
    {
        qWarning() << "Udp request received with no pending data!";
        return;
    }

    QByteArray data;
    QHostAddress peerAddress;

    data.resize(int(udpSocket->pendingDatagramSize()));
    udpSocket->readDatagram(data.data(), data.size(), &peerAddress);

    qDebug() << "Received command: " << data;
    if (data == "DiscoverFireSensors")
    {
        sendServerAddressToRequester(peerAddress);
        return;
    }

    qDebug() << "Unknown command, ignoring...";
}

void DetectionService::sendServerAddressToRequester(const QHostAddress &requesterAddress)
{
    quint16 reqPort = 56560;
    qDebug() << "Replying with my IP and port to requester: " << requesterAddress.toString();

    TcpClient tcpClient;
    auto message = "DiscoveryReply - " + serverAddress.toString().toStdString() + ";" + std::to_string(serverPort);
    tcpClient.sendRequest(requesterAddress, reqPort, message);
}

DetectionService::~DetectionService() = default;
