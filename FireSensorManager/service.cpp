#include "service.h"

#include "sensorstate.h"
#include "Communication/sensorcommunication.h"

#include <algorithm>
#include <QRandomGenerator>

Service::Service(QObject *parent)
    : QObject{parent}, sensorCommunication(new SensorCommunication())
{ }

int Service::getTemperature()
{
    auto knownSensors = sensorCommunication->getKnownSensors();
    if (knownSensors.size() == 0)
    {
        qDebug() << "No known sensors to send the request to, aborting";
        return -1;
    }

    std::vector<std::shared_ptr<SensorState>> temperatureSupportingSensors;
    std::copy_if(knownSensors.begin(),
                 knownSensors.end(),
                 std::back_inserter(temperatureSupportingSensors),
                 [&](const std::shared_ptr<SensorState>& sensor) { return sensor->getCapabilities().testFlag(Capability::Temperature); });
    if (temperatureSupportingSensors.empty())
    {
        qDebug() << "No sensor supports temperature!";
        return -1;
    }

    auto rand = QRandomGenerator::global()->bounded(0, (int)temperatureSupportingSensors.size());
    auto sensor = temperatureSupportingSensors[rand];

    auto dataUpdated = sensorCommunication->updateData(*sensor);
    if (!dataUpdated)
        return -1;

    auto temperature = sensor->getTemperature();
    qDebug() << "User asked for temperature, returing: " << temperature;
    return temperature;
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
