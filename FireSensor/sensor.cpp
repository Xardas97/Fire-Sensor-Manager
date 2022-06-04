#include "sensor.h"

#include "ports.h"
#include "tcpserver.h"
#include "tcpmessages.h"
#include "sensorstate.h"
#include "detectionservice.h"

#include <QDebug>
#include <QTcpSocket>
#include <QNetworkInterface>

Sensor::Sensor(QObject *parent)
    : QObject{parent}, m_tcpServer(new TcpServer())
{
    QObject::connect(m_tcpServer.get(), &TcpServer::onReceivedCommand, this, &Sensor::onReceivedCommand);
}

bool Sensor::startSensor(Capabilities capabilities)
{
    auto localAddress = getLocalAddress();

    for (int i = 0; i < Ports::max_sensor_ports; ++i)
    {
        auto started = m_tcpServer->startServer(localAddress, Ports::base_sensor_port + i);
        if (started) break;
    }

    if (!m_tcpServer->isListening())
    {
        qWarning() << "Failed to start the server on any of the ports!";
        return false;
    }

    m_sensorState = std::make_shared<SensorState>(capabilities, m_tcpServer->serverAddress(), m_tcpServer->serverPort());
    emit sensorStateChanged();

    m_detectionService = std::make_unique<DetectionService>(m_sensorState);

    return true;
}

void Sensor::stopSensor()
{
    m_tcpServer->stopServer();
    m_detectionService = nullptr;
    m_sensorState = nullptr;
    emit sensorStateChanged();
}

void Sensor::onReceivedCommand(const TcpSocket& socket, const QJsonObject& data)
{
    if (data["command"] == TcpMessages::Command::GetData["command"])
    {
        qDebug() << "Client asked for sensor data, returing temperature: " << m_sensorState->temperature();

        auto response = TcpMessages::Response::Ack;
        response["data"] = m_sensorState->toDataJson();
        socket.write(response);
        qDebug() << "Sensor data written";

        return;
    }

    if (data["command"] == TcpMessages::Command::SetName["command"])
    {
        qDebug() << "Client wants to change sensor name";

        if (!data.contains("name")) {
            qWarning() << "Name field missing!";
            socket.write(TcpMessages::Response::BrokenData);
            return;
        }

        auto newName = data["name"].toString();
        qDebug() << "Changing name to: " << newName;
        m_sensorState->setName(newName);

        auto response = TcpMessages::Response::Ack;
        socket.write(response);

        return;
    }

    if (data["command"] == TcpMessages::Command::Identify["command"])
    {
        qDebug() << "Clients requested identification";

        QJsonObject response = TcpMessages::Response::Ack;
        response["data"] = m_sensorState->toIdentityJson();

        socket.write(response);
        return;
    }

    socket.write(TcpMessages::Response::CommandNotRecognized);
    qDebug() << "Command unknown, error response returned!";
}

QHostAddress Sensor::getLocalAddress()
{
    auto addresses = QNetworkInterface::allAddresses();
    for (const auto &address: addresses)
    {
        if (address.isLoopback())
            continue;

        if (address.protocol() != QAbstractSocket::NetworkLayerProtocol::IPv4Protocol)
            continue;

        return address;
    }

    return QHostAddress::Null;
}

SensorState* Sensor::sensorState() const
{
    return m_sensorState.get();
}

Sensor::~Sensor() = default;
