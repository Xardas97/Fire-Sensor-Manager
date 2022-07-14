#include "service.h"

#include "database.h"
#include "Map/floormaps.h"
#include "Map/mapimageprovider.h"
#include "Communication/sensorcommunication.h"

#include <QHostAddress>

Service::Service(QObject *parent)
    : QObject{parent}, m_activeUserProfile{"admin"},
      m_database(new Database()),
      m_floorMaps(new FloorMaps(m_database)),
      m_usersModel(new UsersModel(m_database)),
      m_alarmManager(new AlarmManager(m_database, "admin")),
      m_sensorCommunication(new SensorCommunication(m_database)),
      m_warningTracker(new WarningTracker(m_floorMaps, m_sensorCommunication->knownSensors())),
      m_knownSensorsFilterModel(new FilteredSensorListModel())
{
    m_knownSensorsFilterModel->setSourceModel(m_sensorCommunication->knownSensors().get());
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorAdded, this, &Service::onFloorAdded);
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorRemoved, this, &Service::onFloorRemoved);
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorPartAdded, this, &Service::onFloorPartAdded);
    QObject::connect(m_floorMaps.get(), &FloorMaps::floorPartRemoved, this, &Service::onFloorPartRemoved);

    QObject::connect(m_usersModel.get(), &UsersModel::loggedUserChanged, this, &Service::onLoggedUserChanged);

    loadUserProfile();
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

    auto foundSensor = m_sensorCommunication->knownSensors()->find(*sensor);
    if (!foundSensor)
        return false;

    mapEntry->addSensor(foundSensor);

    return true;
}

void Service::removeFromMap(Sensor* sensor)
{
    auto foundSensor = m_sensorCommunication->knownSensors()->find(*sensor);
    if (!foundSensor)
        return;

    auto map = foundSensor->map();
    if (map) {
        map->removeSensor(foundSensor);
    }
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

    auto foundSensor = m_sensorCommunication->knownSensors()->find(*sensor);
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

bool Service::floorExists(QVariant floor)
{
    if (floor.isNull())
        return false;

    return m_floorMaps->floorSize(floor.toInt()) > 0;
}

bool Service::selectedFloorPartExists()
{
    if (!m_selectedFloor.has_value() || !m_selectedFloorPart.has_value())
        return false;

    return m_floorMaps->floorSize(*m_selectedFloor) > *m_selectedFloorPart;
}

QVariant Service::selectedFloorSize()
{
    if (!m_selectedFloor)
        return QVariant{};

    return m_floorMaps->floorSize(*m_selectedFloor);
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

void Service::loadUserProfile()
{
    auto settings = m_database->loadUserSettings(m_activeUserProfile);
    setUITheme(settings.uiTheme);
    setUIAccent(settings.uiAccent);
}

void Service::saveUserProfile()
{
    m_database->saveAppearanceData(m_activeUserProfile, m_uiTheme, m_uiAccent);
}

int Service::uiTheme() const
{
    return m_uiTheme;
}

int Service::uiAccent() const
{
    return m_uiAccent;
}

void Service::setUITheme(int uiTheme)
{
    if (m_uiTheme != uiTheme)
    {
        m_uiTheme = uiTheme;
        emit uiThemeChanged();
    }
}

void Service::setUIAccent(int uiAccent)
{
    if (m_uiAccent != uiAccent)
    {
        m_uiAccent = uiAccent;
        emit uiAccentChanged();
    }
}

UsersModel* Service::usersModel()
{
    return m_usersModel.get();
}

AlarmManager* Service::alarmManager()
{
    return m_alarmManager.get();
}

WarningTracker* Service::warningTracker()
{
    return m_warningTracker.get();
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

void Service::onLoggedUserChanged()
{
    auto newUserProfile = m_usersModel->isLoggedIn()
                           ? m_usersModel->loggedUsername()
                           : "admin";

    m_alarmManager->setActiveUserProfile(newUserProfile);

    if (newUserProfile != m_activeUserProfile)
    {
        saveUserProfile();
        m_activeUserProfile = newUserProfile;
        loadUserProfile();
    }
}

Service::~Service()
{
    saveUserProfile();
}
