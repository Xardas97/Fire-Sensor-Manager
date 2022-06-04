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

    Q_PROPERTY(FilteredSensorListModel* knownSensorsModel READ getKnownSensorsFilterModel CONSTANT)

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    void discoverSensor(const QString& address, quint16 port);
    void discoverSensors();

    FilteredSensorListModel* getKnownSensorsFilterModel();

private:
    std::unique_ptr<SensorCommunication> sensorCommunication;
    std::unique_ptr<FilteredSensorListModel> knownSensorsFilterModel;
};

#endif // SERVICE_H
