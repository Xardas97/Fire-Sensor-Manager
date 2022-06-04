#include "service.h"

#include "Communication/sensorcommunication.h"

#include <QHostAddress>

Service::Service(QObject *parent)
    : QObject{parent},
      m_sensorCommunication(new SensorCommunication()),
      m_knownSensorsFilterModel(new FilteredSensorListModel())
{
    m_knownSensorsFilterModel->setSourceModel(&m_sensorCommunication->knownSensors());
}

void Service::discoverSensor(const QString& address, quint16 port)
{
    m_sensorCommunication->discoverSensor(QHostAddress{address}, port);
}

void Service::discoverSensors()
{
    m_sensorCommunication->discoverSensors();
}

FilteredSensorListModel* Service::knownSensorsFilterModel()
{
    return m_knownSensorsFilterModel.get();
}

Service::~Service() = default;
