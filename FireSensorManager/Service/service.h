#ifndef SERVICE_H
#define SERVICE_H

#include "Communication/sensor.h"
#include "filteredsensorlistmodel.h"

#include <QList>
#include <memory>
#include <QObject>
#include <optional>

class Sensor;
class Database;
class FloorMaps;
class MapImageProvider;
class SensorCommunication;
enum Permissions : unsigned short;

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FilteredSensorListModel* knownSensorsModel READ knownSensorsFilterModel CONSTANT)
    Q_PROPERTY(QList<Sensor*> alarmedPlacedSensors    READ alarmedPlacedSensors    NOTIFY alarmedPlacedSensorsChanged)
    Q_PROPERTY(QList<Sensor*> placedSensorsWithStatus READ placedSensorsWithStatus NOTIFY placedSensorsWithStatusChanged)

    Q_PROPERTY(QVariant selectedFloor     READ selectedFloor     WRITE setSelectedFloor     NOTIFY selectedFloorChanged)
    Q_PROPERTY(QVariant selectedFloorPart READ selectedFloorPart WRITE setSelectedFloorPart NOTIFY selectedFloorPartChanged)

    Q_PROPERTY(QStringList availableFloors     READ availableFloors     NOTIFY availableFloorsChanged)
    Q_PROPERTY(QStringList availableFloorParts READ availableFloorParts NOTIFY availableFloorPartsChanged)

    Q_PROPERTY(bool    isLoggedIn          READ isLoggedIn          NOTIFY loggedUserChanged)
    Q_PROPERTY(QString loggedUsername      READ loggedUsername      NOTIFY loggedUserChanged)
    Q_PROPERTY(bool    hasModPermissions   READ hasModPermissions   NOTIFY loggedUserChanged)
    Q_PROPERTY(bool    hasAdminPermissions READ hasAdminPermissions NOTIFY loggedUserChanged)

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

    QList<Sensor*> alarmedPlacedSensors();
    QList<Sensor*> placedSensorsWithStatus();

    QVariant selectedFloor();
    QVariant selectedFloorPart();

    QStringList availableFloors();
    QStringList availableFloorParts();

    bool    isLoggedIn() const;
    QString loggedUsername() const;
    bool    hasModPermissions() const;
    bool    hasAdminPermissions() const;

    MapImageProvider*        createMapImageProvider();
    FilteredSensorListModel* knownSensorsFilterModel();

public slots:
    void discoverSensor(const QString& address, quint16 port);
    void discoverSensors();

    bool updateName(Sensor* sensor, const QString& name);

    void removeSensor(Sensor* sensor);
    bool reactivateSensor(Sensor* sensor);

    bool placeOnMap(Sensor* sensor);
    void removeFromMap(Sensor* sensor);
    QList<Sensor*> currentMapSensors();


    void setSelectedFloor(QVariant floor);
    void setSelectedFloorPart(QVariant floorPart);
    bool selectFloorPartThatContains(Sensor* sensor);
    bool floorPartExists(QVariant floor, QVariant floorPart);

    bool uploadMap(int floor, const QUrl& url);
    void removeFloor(int floor);


    bool logIn(QString username, QString passphrase);
    void logOut();

    bool addUser(QString username, QString passphrase, Permissions permissions);
    bool updateUserPassphrase(QString username, QString passphrase);
    bool updateUserPermissions(QString username, Permissions permissions);
    bool removeUser(QString username);

signals:
    void floorAdded(int floor);
    void floorRemoved(int floor);
    void floorPartAdded(int floor);
    void floorPartRemoved(int floor);

    void alarmedPlacedSensorsChanged();
    void placedSensorsWithStatusChanged();

    void selectedFloorChanged();
    void selectedFloorPartChanged();

    void availableFloorsChanged();
    void availableFloorPartsChanged();

    void loggedUserChanged();

private:
    void onFloorAdded(int floor);
    void onFloorRemoved(int floor);
    void onFloorPartAdded(int floor);
    void onFloorPartRemoved(int floor);

    std::optional<int> m_selectedFloor;
    std::optional<short> m_selectedFloorPart;

    std::optional<QString> m_loggedUsername;
    Permissions            m_permissions;

    std::shared_ptr<Database>                m_database;
    std::shared_ptr<FloorMaps>               m_floorMaps;
    std::unique_ptr<SensorCommunication>     m_sensorCommunication;
    std::unique_ptr<FilteredSensorListModel> m_knownSensorsFilterModel;
};

#endif // SERVICE_H
