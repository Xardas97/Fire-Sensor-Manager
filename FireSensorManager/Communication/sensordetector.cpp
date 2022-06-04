#include "sensordetector.h"

#include "ports.h"
#include "sensor.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "tcpmessages.h"

#include <QUdpSocket>
#include <QTcpSocket>

SensorDetector::SensorDetector()
    : m_tcpServer(new TcpServer())
{
    QObject::connect(m_tcpServer.get(), &TcpServer::onReceivedCommand, this, &SensorDetector::onReceivedCommand);
}

void SensorDetector::discoverSensors()
{
    qDebug() << "Discovering sensors...";

    if (!m_tcpServer->isListening())
    {
        qDebug() << "Starting discovering TcpServer...";
        bool started = m_tcpServer->startServer(QHostAddress::Any, Ports::sensor_detector_port);
        if (!started) return;
    }

    QUdpSocket udpSocket;

    for (int i = 0; i < Ports::max_sensor_ports; ++i)
    {
        // Sending broadcast to multiple ports to support simulating multiple sensors on same machine
        auto command = TcpMessages::getBytes(TcpMessages::Command::DiscoverSensors);
        udpSocket.writeDatagram(command, QHostAddress::Broadcast, Ports::base_sensor_port + i);
    }

    qDebug() << "Broadcast message sent!";
}

bool SensorDetector::discoverSensor(const QHostAddress& address, quint16 port)
{
    TcpClient tcpClient;
    auto response = tcpClient.sendRequest(address, port, TcpMessages::Command::Identify);
    return parseIncomingSensorIdentification(response);
}

void SensorDetector::onReceivedCommand(const TcpSocket& socket, const QJsonObject& command)
{
    if (command["command"] == TcpMessages::Command::Identify["command"])
    {
        auto success = parseIncomingSensorIdentification(command);
        if (success)
        {
            socket.write(TcpMessages::Response::Ack);
        }
        else
        {
            qWarning() << "Sensor returned no data!";
            socket.write(TcpMessages::Response::BrokenData);
        }

        return;
    }

    socket.write(TcpMessages::Response::CommandNotRecognized);
    qDebug() << "Command unknown, error response returned!";
}

bool SensorDetector::parseIncomingSensorIdentification(const QJsonObject& json)
{
    if (!json.contains("data"))
    {
        qWarning() << "Sensor returned no data!";
        return false;
    }

    auto sensor = Sensor::fromJson(json["data"].toObject());
    emit onSensorDiscovered(std::move(sensor));
    return true;
}

SensorDetector::~SensorDetector() = default;
