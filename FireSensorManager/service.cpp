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
    QObject::connect(m_mapImageProvider.get(), &MapImageProvider::floorPartAdded, this, &Service::onFloorPartAdded);
    QObject::connect(m_mapImageProvider.get(), &MapImageProvider::floorPartRemoved, this, &Service::onFloorPartRemoved);
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

void Service::removeFloor(int floor)
{
    m_mapImageProvider->removeFloor(floor);
}

QVariant Service::selectedFloor()
{
    if (!m_selectedFloor)
        return QVariant{};

    return *m_selectedFloor;
}

QVariant Service::selectedFloorPart()
{
    if (!m_selectedFloorPart)
        return QVariant{};

    return *m_selectedFloorPart;
}

void Service::setSelectedFloor(QVariant floor)
{
    if (!floor.isNull())
        m_selectedFloor = std::make_unique<int>(floor.toInt());
    else
        m_selectedFloor = nullptr;

    emit selectedFloorChanged();
    emit availableFloorPartsChanged();
}

void Service::setSelectedFloorPart(QVariant floorPart)
{
    if (!floorPart.isNull())
        m_selectedFloorPart = std::make_unique<int>(floorPart.toInt());
    else
        m_selectedFloorPart = nullptr;

    emit selectedFloorPartChanged();
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

QStringList Service::availableFloorParts()
{
    if (!m_selectedFloor)
        return QStringList {};

    auto floorSize = m_mapImageProvider->floorSize(*m_selectedFloor);

    QStringList availableFloorParts;
    for (int i = 0; i < floorSize; ++i)
    {
        availableFloorParts.append(QString::number(i));
    }

    return availableFloorParts;
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

void Service::onFloorPartAdded(int floor)
{
    emit availableFloorPartsChanged();
    emit floorPartAdded(floor);
}

void Service::onFloorPartRemoved(int floor)
{
    emit availableFloorPartsChanged();
    emit floorPartRemoved(floor);
}

Service::~Service() = default;
