#ifndef SERVICE_H
#define SERVICE_H

#include "filteredsensorlistmodel.h"

#include <memory>
#include <QObject>

class Sensor;
class SensorCommunication;

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FilteredSensorListModel* knownSensorsModel READ knownSensorsFilterModel CONSTANT)

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    void discoverSensor(const QString& address, quint16 port);
    void discoverSensors();

    bool updateName(Sensor* sensor, const QString& name);

    FilteredSensorListModel* knownSensorsFilterModel();

private:
    std::unique_ptr<SensorCommunication>     m_sensorCommunication;
    std::unique_ptr<FilteredSensorListModel> m_knownSensorsFilterModel;
};

#endif // SERVICE_H
