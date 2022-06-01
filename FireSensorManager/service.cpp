#include "service.h"

#include "Communication/sensorcommunication.h"

#include <algorithm>
#include <QRandomGenerator>

Service::Service(QObject *parent)
    : QObject{parent}, sensorCommunication(new SensorCommunication())
{ }

Sensor* Service::getRandomSensor()
{
    auto knownSensors = sensorCommunication->getKnownSensors();
    if (knownSensors.size() == 0)
    {
        qDebug() << "No known sensors to send the request to, aborting";
        return nullptr;
    }

    std::vector<std::shared_ptr<Sensor>> temperatureSupportingSensors;
    std::copy_if(knownSensors.begin(),
                 knownSensors.end(),
                 std::back_inserter(temperatureSupportingSensors),
                 [&](const std::shared_ptr<Sensor>& sensor) { return sensor->getCapabilities().testFlag(Capability::Temperature) && !sensor->getIsReplaced(); });
    if (temperatureSupportingSensors.empty())
    {
        qDebug() << "No sensor supports temperature!";
        return nullptr;
    }

    auto rand = QRandomGenerator::global()->bounded(0, (int)temperatureSupportingSensors.size());
    auto sensor = temperatureSupportingSensors[rand];

    qDebug() << "Returning sensor: " << sensor->getName();

    return sensor.get();
}

void Service::discoverSensor(const QString& address, quint16 port)
{
    sensorCommunication->discoverSensor(QHostAddress{address}, port);
}

void Service::discoverSensors()
{
    sensorCommunication->discoverSensors();
}

Service::~Service() = default;
