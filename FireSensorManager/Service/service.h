#ifndef SERVICE_H
#define SERVICE_H

#include "usersmodel.h"
#include "warningtracker.h"
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

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(UsersModel* users READ usersModel CONSTANT)
    Q_PROPERTY(WarningTracker* warningTracker READ warningTracker CONSTANT)
    Q_PROPERTY(FilteredSensorListModel* knownSensorsModel READ knownSensorsFilterModel CONSTANT)

    Q_PROPERTY(QVariant selectedFloor     READ selectedFloor     WRITE setSelectedFloor     NOTIFY selectedFloorChanged)
    Q_PROPERTY(QVariant selectedFloorPart READ selectedFloorPart WRITE setSelectedFloorPart NOTIFY selectedFloorPartChanged)
    Q_PROPERTY(QVariant selectedFloorSize READ selectedFloorSize NOTIFY availableFloorPartsChanged)

    Q_PROPERTY(QStringList availableFloors     READ availableFloors     NOTIFY availableFloorsChanged)
    Q_PROPERTY(QStringList availableFloorParts READ availableFloorParts NOTIFY availableFloorPartsChanged)

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

    QVariant selectedFloor();
    QVariant selectedFloorPart();
    QVariant selectedFloorSize();

    QStringList availableFloors();
    QStringList availableFloorParts();

    UsersModel*              usersModel();
    WarningTracker*          warningTracker();
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

    bool floorExists(QVariant floor);
    bool selectedFloorPartExists();
    bool floorPartExists(QVariant floor, QVariant floorPart);

    bool uploadMap(int floor, const QUrl& url);
    void removeFloor(int floor);

signals:
    void floorAdded(int floor);
    void floorRemoved(int floor);
    void floorPartAdded(int floor);
    void floorPartRemoved(int floor);

    void selectedFloorChanged();
    void selectedFloorPartChanged();

    void availableFloorsChanged();
    void availableFloorPartsChanged();

private:
    void onFloorAdded(int floor);
    void onFloorRemoved(int floor);
    void onFloorPartAdded(int floor);
    void onFloorPartRemoved(int floor);

    std::optional<int> m_selectedFloor;
    std::optional<short> m_selectedFloorPart;

    std::shared_ptr<Database>                m_database;
    std::shared_ptr<FloorMaps>               m_floorMaps;
    std::unique_ptr<UsersModel>              m_usersModel;
    std::unique_ptr<SensorCommunication>     m_sensorCommunication;
    std::unique_ptr<WarningTracker>          m_warningTracker;
    std::unique_ptr<FilteredSensorListModel> m_knownSensorsFilterModel;
};

#endif // SERVICE_H
