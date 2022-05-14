#include "detectionservice.h"

#include "ports.h"
#include "tcpclient.h"
#include "tcpmessages.h"

#include <QDebug>
#include <QUdpSocket>
#include <QJsonDocument>

DetectionService::DetectionService(const QHostAddress& serverAddress, quint16 serverPort, QObject *parent)
    : QObject{parent}, serverAddress(serverAddress), serverPort(serverPort), udpSocket(new QUdpSocket())
{
    udpSocket->bind(serverPort, QUdpSocket::ShareAddress);
    QObject::connect(udpSocket.get(), &QUdpSocket::readyRead, this, &DetectionService::processDetectionRequest);
    qDebug() << "DetectionService listening on port " << serverPort;
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
    QJsonDocument dataDoc = QJsonDocument::fromJson(data);
    if (dataDoc["command"] == TcpMessages::Command::DiscoverSensors["command"])
    {
        sendServerAddressToRequester(peerAddress);
        return;
    }

    qDebug() << "Unknown command, ignoring...";
}

void DetectionService::sendServerAddressToRequester(const QHostAddress &requesterAddress)
{
    qDebug() << "Replying with my IP and port to requester: " << requesterAddress.toString();

    QJsonObject message = TcpMessages::Command::Identify;
    message["address"] =  serverAddress.toString();
    message["port"] = serverPort;

    TcpClient tcpClient;
    tcpClient.sendRequest(requesterAddress, Ports::sensorDetectorPort, message);
}

DetectionService::~DetectionService() = default;
