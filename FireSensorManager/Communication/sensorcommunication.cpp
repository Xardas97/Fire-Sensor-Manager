#include "sensorcommunication.h"

#include "sensor.h"
#include "database.h"
#include "tcpclient.h"
#include "tcpmessages.h"
#include "sensordetector.h"

#include <QTimer>
#include <QtConcurrent>

SensorCommunication::SensorCommunication(std::shared_ptr<Database> database)
    : m_sensorDetector(new SensorDetector())
{
    m_database = database;
    for (auto& sensor: m_database->loadSensors())
    {
        m_knownSensors.add(std::move(sensor));
    }

    QObject::connect(m_sensorDetector.get(), &SensorDetector::onSensorDiscovered, this, &SensorCommunication::onSensorDiscovered);
    QTimer::singleShot(5000, this, &SensorCommunication::updateSensors);
}

void SensorCommunication::updateSensors()
{
    m_sensorUpdatesCount = (m_sensorUpdatesCount + 1) % check_inactive_sensors_count;
    bool updateInactiveSensors = m_sensorUpdatesCount == 0;

    if (updateInactiveSensors)
        qDebug() << "Updating inactive sensors...";

    for (auto& sensor: m_knownSensors.sensors())
    {
        if (sensor->isReplaced())
            continue;

        if (!sensor->isActive() && !updateInactiveSensors)
            continue;

        auto sensorUpdateFuture = QtConcurrent::run([&]()
        {
            auto updated = updateData(*sensor);
            if (updated)
                sensor->reportCommunicationSuccess();
            else
                sensor->reportCommunicationFailure();
        });
    }

    QTimer::singleShot(5000, this, &SensorCommunication::updateSensors);
}

bool SensorCommunication::updateData(Sensor& sensor)
{
    TcpClient tcpClient;

    auto address = sensor.address();
    auto port = sensor.port();

    auto response = tcpClient.sendRequest(address, port, TcpMessages::Command::GetData);
    if (!response.contains("data"))
    {
        return false;
    }

    auto sameSensor = sensor.updateData(response["data"].toObject());
    if (!sameSensor)
    {
        qDebug() << "New sensor on this IP address, setting replaced!";
        sensor.setIsReplaced();
        return false;
    }

    return true;
}

bool SensorCommunication::updateName(Sensor& sensor, const QString& name)
{
    qDebug() << "Updating name of the sensor " << sensor.name() << " to " << name;
    TcpClient tcpClient;

    auto address = sensor.address();
    auto port = sensor.port();

    auto command = TcpMessages::Command::SetName;
    command["name"] = name;

    auto response = tcpClient.sendRequest(address, port, command);

    if (response != TcpMessages::Response::Ack)
    {
        qDebug() << "Failed to update name of the sensor";
        return false;
    }

    sensor.setName(name);
    return true;
}

SensorList& SensorCommunication::knownSensors()
{
    return m_knownSensors;
}

void SensorCommunication::discoverSensors()
{
    m_sensorDetector->discoverSensors();
}

void SensorCommunication::discoverSensor(const QHostAddress &address, quint16 port)
{
    m_sensorDetector->discoverSensor(address, port);
}

void SensorCommunication::onSensorDiscovered(std::shared_ptr<Sensor> sensor)
{
    qDebug() << "Discovered sensor: " << *sensor;

    auto found = m_knownSensors.find(sensor);
    if (found == nullptr)
    {
        qDebug() << "This is a new sensor!";
        m_knownSensors.add(sensor);
        return;
    }

    if (sensor->address() != found->address() || sensor->port() != found->port())
    {
        qDebug() << "Known sensor found on different address, replacing!";
        m_knownSensors.remove(found);
        m_knownSensors.add(sensor);
    }
}

SensorCommunication::~SensorCommunication()
{
    m_database->saveSensors(m_knownSensors.sensors());
}
