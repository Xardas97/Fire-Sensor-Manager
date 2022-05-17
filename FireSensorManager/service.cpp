#include "service.h"

#include "ports.h"
#include "tcpclient.h"
#include "tcpmessages.h"
#include "sensorstate.h"
#include "Communication/firesensordetector.h"

#include <QDebug>
#include <QTcpSocket>
#include <QRandomGenerator>

Service::Service(QObject *parent)
    : QObject{parent}, fireSensorDetector(new FireSensorDetector())
{
    QObject::connect(fireSensorDetector.get(), &FireSensorDetector::onSensorDiscovered, this, &Service::onSensorDiscovered);
}

int Service::getTemperature()
{
    TcpClient tcpClient;

    if (knownSensors.size() == 0)
    {
        qDebug() << "No known sensors to send the request to, aborting";
        return -1;
    }

    auto rand = QRandomGenerator::global()->bounded(0, (int)knownSensors.size());
    auto sensor = knownSensors[rand];

    auto address = sensor->getAddress();
    auto port = sensor->getPort();

    auto response = tcpClient.sendRequest(address, port, TcpMessages::Command::GetData);
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

void Service::onSensorDiscovered(std::shared_ptr<SensorState> sensor)
{
    qDebug() << "Discovered sensor: " << *sensor;

    auto found = std::find_if(knownSensors.cbegin(),
                              knownSensors.cend(),
                              [&sensor](const std::shared_ptr<SensorState> knownSensor) {return knownSensor->getUuid() == sensor->getUuid(); });
    if (found == knownSensors.cend()) {
        qDebug() << "This is a new sensor!";
        knownSensors.push_back(sensor);
    }
}

Service::~Service() = default;
