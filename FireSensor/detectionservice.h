#ifndef DETECTIONSERVICE_H
#define DETECTIONSERVICE_H

#include <QObject>
#include <memory>

class QUdpSocket;

class DetectionService : public QObject
{
    Q_OBJECT
public:
    explicit DetectionService(QObject *parent = nullptr);
    ~DetectionService();

private slots:
    void processDetectionRequest();

private:
    std::unique_ptr<QUdpSocket> udpSocket;
};

#endif // DETECTIONSERVICE_H
