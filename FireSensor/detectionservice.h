#ifndef DETECTIONSERVICE_H
#define DETECTIONSERVICE_H

#include <memory>
#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class SensorState;

class DetectionService : public QObject
{
    Q_OBJECT
public:
    explicit DetectionService(std::shared_ptr<SensorState> sensorState, QObject *parent = nullptr);
    ~DetectionService();

private:
    void processDetectionRequest();
    void sendServerAddressToRequester(const QHostAddress& requesterAddress);

    std::shared_ptr<SensorState> sensorState;
    std::unique_ptr<QUdpSocket> udpSocket;
};

#endif // DETECTIONSERVICE_H
