#include "sensorcommunication.h"

#include "sensor.h"
#include "tcpclient.h"
#include "tcpmessages.h"
#include "sensordetector.h"

#include <QTimer>
#include <QtConcurrent>

SensorCommunication::SensorCommunication()
    : fireSensorDetector(new SensorDetector())
{
    QObject::connect(fireSensorDetector.get(), &SensorDetector::onSensorDiscovered, this, &SensorCommunication::onSensorDiscovered);
    QTimer::singleShot(5000, this, &SensorCommunication::updateSensors);
}

void SensorCommunication::updateSensors()
{
    sensorUpdatesCount = (sensorUpdatesCount + 1) % checkInactiveSensorsCount;
    bool updateInactiveSensors = sensorUpdatesCount == 0;

    if (updateInactiveSensors)
        qDebug() << "Updating inactive sensors...";

    for (auto& sensor: knownSensors.getSensors())
    {
        if (sensor->getIsReplaced())
            continue;

        if (!sensor->getIsActive() && !updateInactiveSensors)
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

    auto address = sensor.getAddress();
    auto port = sensor.getPort();

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

    sensor.updateData(response["data"].toObject());
    return true;
}

SensorList& SensorCommunication::getKnownSensors()
{
    return knownSensors;
}

void SensorCommunication::discoverSensors()
{
    fireSensorDetector->discoverSensors();
}

void SensorCommunication::discoverSensor(const QHostAddress &address, quint16 port)
{
    fireSensorDetector->discoverSensor(address, port);
}

void SensorCommunication::onSensorDiscovered(std::shared_ptr<Sensor> sensor)
{
    qDebug() << "Discovered sensor: " << *sensor;

    auto found = knownSensors.find(sensor);
    if (found == nullptr)
    {
        qDebug() << "This is a new sensor!";
        knownSensors.add(sensor);
        return;
    }

    if (sensor->getAddress() != found->getAddress() || sensor->getPort() != found->getPort())
    {
        qDebug() << "Known sensor found on different address, replacing!";
        knownSensors.remove(found);
        knownSensors.add(sensor);
    }
}

SensorCommunication::~SensorCommunication() = default;
