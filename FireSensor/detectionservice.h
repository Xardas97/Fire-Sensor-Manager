#ifndef DETECTIONSERVICE_H
#define DETECTIONSERVICE_H

#include <memory>
#include <QObject>
#include <QHostAddress>

class QUdpSocket;

class DetectionService : public QObject
{
    Q_OBJECT
public:
    explicit DetectionService(const QHostAddress& serverAddress, quint16 serverPort, QObject *parent = nullptr);
    ~DetectionService();

private slots:
    void processDetectionRequest();

private:
    QHostAddress serverAddress;
    quint16 serverPort;

    std::unique_ptr<QUdpSocket> udpSocket;

    void sendServerAddressToRequester(const QHostAddress& requesterAddress);
};

#endif // DETECTIONSERVICE_H
