#include "sensor.h"

#include "ports.h"
#include "tcpserver.h"
#include "tcpmessages.h"
#include "sensorstate.h"
#include "detectionservice.h"

#include <QFile>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>
#include <QNetworkInterface>

Sensor::Sensor(QObject *parent)
    : QObject{parent}, m_tcpServer(new TcpServer())
{
    QObject::connect(m_tcpServer.get(), &TcpServer::onReceivedCommand, this, &Sensor::onReceivedCommand);
}

bool Sensor::startSensor(Capabilities capabilities)
{
    if (!startServer())
        return false;

    m_sensorState = std::make_shared<SensorState>(capabilities, m_tcpServer->serverAddress(), m_tcpServer->serverPort());
    m_detectionService = std::make_unique<DetectionService>(m_sensorState);
    emit sensorStateChanged();

    return true;
}

void Sensor::stopSensor()
{
    m_tcpServer->stopServer();
    m_detectionService = nullptr;
    m_sensorState = nullptr;
    emit sensorStateChanged();
}

bool Sensor::loadSensor(const QUrl& url)
{
    qDebug() << "User asked to load sensor from: " << url.toLocalFile();

    QFile file {url.toLocalFile()};
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open the desired file";
        return false;
    }

    QTextStream in(&file);
    auto data = in.readAll();

    file.close();

    QJsonDocument dataDoc = QJsonDocument::fromJson(data.toUtf8());
    auto json = dataDoc.object();

    auto sensor = SensorState::fromJson(json);

    if (!startServer())
        return false;

    sensor->setPort(m_tcpServer->serverPort());
    sensor->setAddress(m_tcpServer->serverAddress());

    m_sensorState = std::move(sensor);
    m_detectionService = std::make_unique<DetectionService>(m_sensorState);
    emit sensorStateChanged();

    qDebug() << "Successfully loaded sensor data";
    return true;
}

bool Sensor::saveSensor(const QUrl& url)
{
    qDebug() << "User asked to save sensor to: " << url.toLocalFile();
    if (!m_sensorState)
        return false;

    QFile file {url.toLocalFile()};
    if (!file.open(QIODevice::WriteOnly | QIODeviceBase::Truncate | QIODevice::Text))
    {
        qWarning() << "Failed to open the desired file";
        return false;
    }

    auto json = m_sensorState->toIdentityJson();
    auto data = QJsonDocument{json}.toJson(QJsonDocument::JsonFormat::Indented);

    QTextStream out(&file);
    out << data;

    file.close();

    qDebug() << "Successfully saved sensor data";
    return true;
}

bool Sensor::startServer()
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

    return true;
}

void Sensor::onReceivedCommand(const TcpSocket& socket, const QJsonObject& data)
{
    if (data["command"] == TcpMessages::Command::GetData["command"])
    {
        qDebug() << "Client asked for sensor data";

        auto response = TcpMessages::Response::Ack;
        response["data"] = m_sensorState->toDataJson();
        socket.write(response);
        qDebug() << "Sensor data written";

        return;
    }

    if (data["command"] == TcpMessages::Command::SetName["command"])
    {
        qDebug() << "Client wants to change sensor name";

        if (!data.contains("name") || !data.contains("uuid")) {
            qWarning() << "Name or uuid field missing!";
            socket.write(TcpMessages::Response::BrokenData);
            return;
        }

        if (data["uuid"] != m_sensorState->uuid().toString())
        {
            qWarning() << "This is not the sensor the caller is expecting!";
            socket.write(TcpMessages::Response::WrongSensor);
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
