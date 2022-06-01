#include "sensorcommunication.h"

#include "tcpclient.h"
#include "tcpmessages.h"
#include "firesensordetector.h"

#include <QTimer>
#include <algorithm>
#include <QtConcurrent>

SensorCommunication::SensorCommunication()
    : fireSensorDetector(new FireSensorDetector())
{
    QObject::connect(fireSensorDetector.get(), &FireSensorDetector::onSensorDiscovered, this, &SensorCommunication::onSensorDiscovered);
    QTimer::singleShot(5000, this, &SensorCommunication::updateSensors);
}

void SensorCommunication::updateSensors()
{
    sensorUpdatesCount = (sensorUpdatesCount + 1) % checkInactiveSensorsCount;
    bool updateInactiveSensors = sensorUpdatesCount == 0;

    if (updateInactiveSensors)
        qDebug() << "Updating inactive sensors...";

    for (auto& sensor: knownSensors)
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

std::vector<std::shared_ptr<Sensor>>& SensorCommunication::getKnownSensors()
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

    auto found = std::find_if(knownSensors.cbegin(),
                              knownSensors.cend(),
                              [&sensor](const std::shared_ptr<Sensor> knownSensor) { return knownSensor->getUuid() == sensor->getUuid(); });

    if (found == knownSensors.cend())
    {
        qDebug() << "This is a new sensor!";
        knownSensors.push_back(sensor);
        return;
    }

    if (sensor->getAddress() != (*found)->getAddress() || sensor->getPort() != (*found)->getPort())
    {
        qDebug() << "Known sensor found on different address, replacing!";
        knownSensors.erase(found);
        knownSensors.push_back(sensor);
    }
}

SensorCommunication::~SensorCommunication() = default;
