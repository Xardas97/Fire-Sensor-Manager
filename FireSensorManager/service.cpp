#include "service.h"

#include "ports.h"
#include "tcpclient.h"
#include "tcpmessages.h"
#include "Communication/firesensordetector.h"

#include <QDebug>
#include <QTcpSocket>

Service::Service(QObject *parent)
    : QObject{parent}, fireSensorDetector(new FireSensorDetector())
{
    QObject::connect(fireSensorDetector.get(), &FireSensorDetector::onSensorDiscovered, this, &Service::onSensorDiscovered);
}

int Service::getNextNumber()
{
    TcpClient tcpClient;

    QHostAddress address = QHostAddress("192.168.1.67");

    auto data = tcpClient.sendRequest(address, Ports::baseSensorPort, TcpMessages::Command::GetNumber);
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
