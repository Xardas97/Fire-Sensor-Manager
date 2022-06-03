#ifndef SERVICE_H
#define SERVICE_H

#include <memory>
#include <QObject>
#include <QAbstractListModel>

class CommunicationSensorState;
class SensorCommunication;

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractListModel* knownSensorsModel READ getKnownSensorsModel)

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    void discoverSensor(const QString& address, quint16 port);
    void discoverSensors();

    QAbstractListModel* getKnownSensorsModel();

private:
    std::unique_ptr<SensorCommunication> sensorCommunication;
};

#endif // SERVICE_H
