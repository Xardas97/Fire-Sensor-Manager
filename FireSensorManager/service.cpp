#include "service.h"

#include <QDebug>
#include <QTcpSocket>
#include "tcpclient.h"
#include "Communication/firesensordetector.h"

Service::Service(QObject *parent)
    : QObject{parent}, fireSensorDetector(new FireSensorDetector())
{
    QObject::connect(fireSensorDetector.get(), &FireSensorDetector::onSensorDiscovered, this, &Service::onSensorDiscovered);
}

int Service::getNextNumber()
{
    TcpClient tcpClient;

    QHostAddress address = QHostAddress("192.168.1.67");
    quint16 port = 56000;

    auto data = tcpClient.sendRequest(address, port, "GET_NUMBER");
    if (data.length() == 0)
    {
        qWarning() << "TCP Communication failed!";
        return -1;
    }

    int nextNumber = data.toInt();
    qDebug() << "User asked for next number, returing: " << nextNumber;
    return nextNumber;
}

void Service::discoverSensors()
{
    fireSensorDetector->discoverSensors();
}

void Service::onSensorDiscovered(const QHostAddress& address, quint16 port)
{
    qDebug() << "Discovered sensor: " << address << "; " << port;
}

Service::~Service() = default;
