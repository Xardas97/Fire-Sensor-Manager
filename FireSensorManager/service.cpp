#include "service.h"

#include "ports.h"
#include "tcpclient.h"
#include "tcpmessages.h"
#include "sensorstate.h"
#include "Communication/firesensordetector.h"

#include <QDebug>
#include <QTcpSocket>

Service::Service(QObject *parent)
    : QObject{parent}, fireSensorDetector(new FireSensorDetector())
{
    QObject::connect(fireSensorDetector.get(), &FireSensorDetector::onSensorDiscovered, this, &Service::onSensorDiscovered);
}

int Service::getTemperature()
{
    TcpClient tcpClient;

    QHostAddress address = QHostAddress("192.168.1.67");

    auto response = tcpClient.sendRequest(address, Ports::baseSensorPort, TcpMessages::Command::GetData);
    if (!response.contains("data"))
    {
        qWarning() << "Sensor returned no data!";
        return -1;
    }

    auto data = response["data"].toObject();
    if (!data.contains("temperature"))
    {
        qWarning() << "Sensor did not return temperature!";
        return -1;
    }

    int temperature = data["temperature"].toInt();
    qDebug() << "User asked for temperature, returing: " << temperature;
    return temperature;
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
