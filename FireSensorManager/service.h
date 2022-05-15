#ifndef SERVICE_H
#define SERVICE_H

#include <memory>
#include <QObject>

class QHostAddress;
class FireSensorDetector;

class Service : public QObject
{
    Q_OBJECT

public:
    explicit Service(QObject *parent = nullptr);
    ~Service();

public slots:
    int getTemperature();
    void discoverSensors();

private:
    void onSensorDiscovered(const QHostAddress& address, quint16 port);

    std::unique_ptr<FireSensorDetector> fireSensorDetector;
};

#endif // SERVICE_H
