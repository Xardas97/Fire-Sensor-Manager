#include "service.h"

#include "database.h"
#include "Map/mapimageprovider.h"
#include "Communication/sensorcommunication.h"

#include <QHostAddress>

Service::Service(QObject *parent)
    : QObject{parent},
      m_database(new Database()),
      m_mapImageProvider(new MapImageProvider(m_database)),
      m_sensorCommunication(new SensorCommunication(m_database)),
      m_knownSensorsFilterModel(new FilteredSensorListModel())
{
    m_knownSensorsFilterModel->setSourceModel(&m_sensorCommunication->knownSensors());
    QObject::connect(m_mapImageProvider.get(), &MapImageProvider::floorAdded, this, &Service::onFloorAdded);
    QObject::connect(m_mapImageProvider.get(), &MapImageProvider::floorRemoved, this, &Service::onFloorRemoved);
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

bool Service::uploadMap(int floor, const QUrl& url)
{
    return m_mapImageProvider->upload(floor, url);
}

QStringList Service::availableFloors()
{
    auto availableFloorsSet = m_mapImageProvider->availableFloors();
    std::vector<int> floorsVector {availableFloorsSet.begin(), availableFloorsSet.end()};
    std::sort(floorsVector.begin(), floorsVector.end());

    QStringList availableFloors;
    for (auto floor: floorsVector)
    {
        availableFloors.append(QString::number(floor));
    }

    return availableFloors;
}

FilteredSensorListModel* Service::knownSensorsFilterModel()
{
    return m_knownSensorsFilterModel.get();
}

MapImageProvider* Service::mapImageProvider()
{
    return m_mapImageProvider.get();
}

void Service::onFloorAdded(int floor)
{
    emit availableFloorsChanged();
    emit floorAdded(floor);
}

void Service::onFloorRemoved(int floor)
{
    emit availableFloorsChanged();
    emit floorRemoved(floor);
}

Service::~Service() = default;
