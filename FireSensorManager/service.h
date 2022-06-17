#ifndef SERVICE_H
#define SERVICE_H

#include "filteredsensorlistmodel.h"

#include <memory>
#include <QObject>

class Sensor;
class Database;
class MapImageProvider;
class SensorCommunication;

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FilteredSensorListModel* knownSensorsModel READ knownSensorsFilterModel CONSTANT)
    Q_PROPERTY(QStringList availableFloors READ availableFloors NOTIFY availableFloorsChanged)

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    void discoverSensor(const QString& address, quint16 port);
    void discoverSensors();

    bool updateName(Sensor* sensor, const QString& name);

    void removeSensor(Sensor* sensor);
    bool reactivateSensor(Sensor* sensor);

    QStringList availableFloors();
    bool uploadMap(int floor, const QUrl& url);

    MapImageProvider*        mapImageProvider();
    FilteredSensorListModel* knownSensorsFilterModel();

signals:
    void availableFloorsChanged();

private:
    std::shared_ptr<Database>                m_database;
    std::unique_ptr<MapImageProvider>        m_mapImageProvider;
    std::unique_ptr<SensorCommunication>     m_sensorCommunication;
    std::unique_ptr<FilteredSensorListModel> m_knownSensorsFilterModel;
};

#endif // SERVICE_H
