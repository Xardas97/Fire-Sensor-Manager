#include "service.h"

#include "database.h"
#include "Map/floormaps.h"
#include "Map/mapimageprovider.h"
#include "Communication/sensorcommunication.h"

#include <QHostAddress>

Service::Service(QObject *parent)
    : QObject{parent}, m_permissions(Permissions::None),
      m_database(new Database()),
      m_floorMaps(new FloorMaps(m_database)),
      m_sensorCommunication(new SensorCommunication(m_database)),
      m_knownSensorsFilterModel(new FilteredSensorListModel())
{
    m_knownSensorsFilterModel->setSourceModel(&m_sensorCommunication->knownSensors());
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorAdded, this, &Service::onFloorAdded);
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorRemoved, this, &Service::onFloorRemoved);
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorPartAdded, this, &Service::onFloorPartAdded);
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorPartRemoved, this, &Service::onFloorPartRemoved);

    QObject::connect(&m_sensorCommunication->knownSensors(), &SensorList::alarmedSensorsChanged, this, &Service::alarmedPlacedSensorsChanged);
    QObject::connect(&m_sensorCommunication->knownSensors(), &SensorList::sensorsWithStatusChanged, this, &Service::placedSensorsWithStatusChanged);
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
    return m_floorMaps->upload(floor, url);
}

void Service::removeFloor(int floor)
{
    m_floorMaps->removeFloor(floor);
}

bool Service::placeOnMap(Sensor* sensor)
{
    auto mapEntry = m_floorMaps->mapEntry(*m_selectedFloor, *m_selectedFloorPart);
    if (!mapEntry)
        return false;

    auto foundSensor = m_sensorCommunication->knownSensors().find(*sensor);
    if (!foundSensor)
        return false;

    mapEntry->addSensor(foundSensor);

    if (foundSensor->alarmOn())
        emit alarmedPlacedSensorsChanged();

    if (foundSensor->status() > 0)
        emit placedSensorsWithStatusChanged();

    return true;
}

void Service::removeFromMap(Sensor* sensor)
{
    auto foundSensor = m_sensorCommunication->knownSensors().find(*sensor);
    if (!foundSensor)
        return;

    auto map = foundSensor->map();
    if (map) {
        map->removeSensor(foundSensor);
    }

    if (foundSensor->alarmOn())
        emit alarmedPlacedSensorsChanged();

    if (foundSensor->status() > 0)
        emit placedSensorsWithStatusChanged();
}

QList<Sensor*> Service::currentMapSensors()
{
    QList<Sensor*> placedSensors;

    if (!m_selectedFloor || !m_selectedFloorPart)
    {
        return placedSensors;
    }

    auto mapEntry = m_floorMaps->mapEntry(*m_selectedFloor, *m_selectedFloorPart);
    if (!mapEntry)
        return placedSensors;

    for (auto& sensor: mapEntry->placedSensors())
        placedSensors.append(sensor.get());

    return placedSensors;
}

QList<Sensor*> Service::alarmedPlacedSensors()
{
    QList<Sensor*> alarmedSensors;

    for (auto& sensor: m_sensorCommunication->knownSensors().sensors())
    {
        if (sensor->alarmOn() && sensor->map())
            alarmedSensors.append(sensor.get());
    }

    return alarmedSensors;
}

QList<Sensor*> Service::placedSensorsWithStatus()
{
    QList<Sensor*> alarmedSensors;

    for (auto& sensor: m_sensorCommunication->knownSensors().sensors())
    {
        if (sensor->status() > 0 && sensor->map())
            alarmedSensors.append(sensor.get());
    }

    return alarmedSensors;
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
        m_selectedFloor = floor.toInt();
    else
        m_selectedFloor.reset();

    emit selectedFloorChanged();
    emit availableFloorPartsChanged();
}

void Service::setSelectedFloorPart(QVariant floorPart)
{
    if (!floorPart.isNull())
        m_selectedFloorPart = floorPart.toInt();
    else
        m_selectedFloorPart.reset();

    emit selectedFloorPartChanged();
}

bool Service::selectFloorPartThatContains(Sensor* sensor)
{
    if (!sensor->map())
        return false;

    auto foundSensor = m_sensorCommunication->knownSensors().find(*sensor);
    if (!foundSensor)
        return false;

    int floor;
    short floorPart;

    auto found = m_floorMaps->getSensorLocation(foundSensor, floor, floorPart);
    if (!found)
        return false;

    setSelectedFloor(floor);
    setSelectedFloorPart(floorPart);
    return true;
}

bool Service::floorPartExists(QVariant floor, QVariant floorPart)
{
    if (floor.isNull() || floorPart.isNull())
        return false;

    return m_floorMaps->floorSize(floor.toInt()) > floorPart.toInt();
}

QStringList Service::availableFloors()
{
    auto availableFloorsSet = m_floorMaps->availableFloors();
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

    auto floorSize = m_floorMaps->floorSize(*m_selectedFloor);

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

MapImageProvider* Service::createMapImageProvider()
{
    return new MapImageProvider(m_floorMaps);
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

bool Service::isLoggedIn() const
{
    return m_loggedUsername.has_value();
}

QString Service::loggedUsername() const
{
    if (!m_loggedUsername.has_value())
        return QString{};

    return *m_loggedUsername;
}

bool Service::hasModPermissions() const
{
    return m_permissions == Permissions::Admin ||
           m_permissions == Permissions::Moderator;
}

bool Service::hasAdminPermissions() const
{
    return m_permissions == Permissions::Admin;
}

bool Service::logIn(QString username, QString passphrase)
{
    auto authenticated = m_database->authenticateUser(username, passphrase, m_permissions);
    if (!authenticated)
        return false;

    m_loggedUsername = username;
    emit loggedUserChanged();
    return true;
}

void Service::logOut()
{
    qDebug() << "Logging out...";

    m_loggedUsername.reset();
    m_permissions = Permissions::None;

    emit loggedUserChanged();
}

bool Service::addUser(QString username, QString passphrase, Permissions permissions)
{
    return m_database->addUser(username, passphrase, permissions);
}

bool Service::updateUserPermissions(QString username, Permissions permissions)
{
    return m_database->updateUserPermissions(username, permissions);
}

bool Service::updateUserPassphrase(QString username, QString oldPassphrase, QString newPassphrase)
{
    Permissions temp;
    auto authenticated = m_database->authenticateUser(username, oldPassphrase, temp);
    if (!authenticated)
        return false;

    return m_database->updateUserPassphrase(username, newPassphrase);
}

bool Service::removeUser(QString username)
{
    return m_database->removeUser(username);
}

Service::~Service() = default;
