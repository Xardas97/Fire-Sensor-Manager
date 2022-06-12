#include "service.h"

#include "database.h"
#include "Communication/sensorcommunication.h"

#include <QHostAddress>

Service::Service(QObject *parent)
    : QObject{parent},
      m_database(new Database()),
      m_sensorCommunication(new SensorCommunication(m_database)),
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

bool Service::updateName(Sensor* sensor, const QString& name)
{
    return m_sensorCommunication->updateName(*sensor, name);
}

void Service::removeSensor(Sensor* sensor)
{
    m_sensorCommunication->removeSensor(*sensor);
}

bool Service::reactivateSensor(Sensor* sensor)
{
    return m_sensorCommunication->reactivateSensor(*sensor);
}

FilteredSensorListModel* Service::knownSensorsFilterModel()
{
    return m_knownSensorsFilterModel.get();
}

Service::~Service() = default;
