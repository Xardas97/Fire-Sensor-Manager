#include "service.h"

#include "Communication/sensorcommunication.h"

#include <QHostAddress>

Service::Service(QObject *parent)
    : QObject{parent}, sensorCommunication(new SensorCommunication()), knownSensorsFilterModel(new FilteredSensorListModel())
{
    knownSensorsFilterModel->setSourceModel(&sensorCommunication->getKnownSensors());
}

void Service::discoverSensor(const QString& address, quint16 port)
{
    sensorCommunication->discoverSensor(QHostAddress{address}, port);
}

void Service::discoverSensors()
{
    sensorCommunication->discoverSensors();
}

FilteredSensorListModel* Service::getKnownSensorsFilterModel()
{
    return knownSensorsFilterModel.get();
}

Service::~Service() = default;
