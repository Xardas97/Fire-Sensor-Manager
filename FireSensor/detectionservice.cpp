#include "detectionservice.h"

#include "ports.h"
#include "tcpclient.h"
#include "tcpmessages.h"
#include "sensorstate.h"

#include <QDebug>
#include <QUdpSocket>
#include <QJsonDocument>

DetectionService::DetectionService(std::shared_ptr<SensorState> sensorState, QObject *parent)
    : QObject{parent}, m_sensorState(sensorState), m_udpSocket(new QUdpSocket())
{
    m_udpSocket->bind(sensorState->port(), QUdpSocket::ShareAddress);
    QObject::connect(m_udpSocket.get(), &QUdpSocket::readyRead, this, &DetectionService::processDetectionRequest);
    qDebug() << "DetectionService listening on port " << sensorState->port();
}

void DetectionService::processDetectionRequest()
{
    qDebug() << "DetectionService received a request";

    if (!m_udpSocket->hasPendingDatagrams())
    {
        qWarning() << "Udp request received with no pending data!";
        return;
    }

    QByteArray data;
    QHostAddress peerAddress;

    data.resize(int(m_udpSocket->pendingDatagramSize()));
    m_udpSocket->readDatagram(data.data(), data.size(), &peerAddress);

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
    qDebug() << "Replying with my identify data: " << requesterAddress.toString();

    QJsonObject message = TcpMessages::Command::Identify;
    message["data"] = m_sensorState->toIdentityJson();

    TcpClient tcpClient;
    tcpClient.sendRequest(requesterAddress, Ports::sensor_detector_port, message);
}

DetectionService::~DetectionService() = default;
