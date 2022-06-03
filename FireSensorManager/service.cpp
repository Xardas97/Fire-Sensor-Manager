#include "service.h"

#include "Communication/sensorcommunication.h"

#include <algorithm>
#include <QRandomGenerator>

Service::Service(QObject *parent)
    : QObject{parent}, sensorCommunication(new SensorCommunication())
{ }

void Service::discoverSensor(const QString& address, quint16 port)
{
    sensorCommunication->discoverSensor(QHostAddress{address}, port);
}

void Service::discoverSensors()
{
    sensorCommunication->discoverSensors();
}

QAbstractListModel* Service::getKnownSensorsModel()
{
    return &sensorCommunication->getKnownSensors();
}

Service::~Service() = default;
