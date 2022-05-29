#include "sensorcommunication.h"

#include "tcpclient.h"
#include "tcpmessages.h"
#include "sensorstate.h"
#include "firesensordetector.h"

#include <algorithm>

SensorCommunication::SensorCommunication()
    : fireSensorDetector(new FireSensorDetector())
{
    QObject::connect(fireSensorDetector.get(), &FireSensorDetector::onSensorDiscovered, this, &SensorCommunication::onSensorDiscovered);
}

bool SensorCommunication::updateData(SensorState& sensor)
{
    TcpClient tcpClient;

    auto address = sensor.getAddress();
    auto port = sensor.getPort();

    auto response = tcpClient.sendRequest(address, port, TcpMessages::Command::GetData);
    if (!response.contains("data"))
    {
        qWarning() << "Sensor returned no data!";
        return false;
    }

    auto sameSensor = sensor.updateData(response["data"].toObject());
    if (!sameSensor)
    {
        // TODO Long term solution is to mark it as "inactive" somehow
        qDebug() << "New sensor on this IP address, removing it from known sensors!";
        knownSensors.erase(std::remove_if(knownSensors.begin(),
                                          knownSensors.end(),
                                          [&sensor](const std::shared_ptr<SensorState> knownSensor) { return knownSensor.get() == &sensor; }),
                           knownSensors.end());
        return false;
    }

    sensor.updateData(response["data"].toObject());
    return true;
}

std::vector<std::shared_ptr<SensorState>>& SensorCommunication::getKnownSensors()
{
    return knownSensors;
}

void SensorCommunication::discoverSensors()
{
    fireSensorDetector->discoverSensors();
}

void SensorCommunication::discoverSensor(const QHostAddress &address, quint16 port)
{
    fireSensorDetector->discoverSensor(address, port);
}

void SensorCommunication::onSensorDiscovered(std::shared_ptr<SensorState> sensor)
{
    qDebug() << "Discovered sensor: " << *sensor;

    auto found = std::find_if(knownSensors.cbegin(),
                              knownSensors.cend(),
                              [&sensor](const std::shared_ptr<SensorState> knownSensor) { return knownSensor->getUuid() == sensor->getUuid(); });
    if (found == knownSensors.cend()) {
        qDebug() << "This is a new sensor!";
        knownSensors.push_back(sensor);
    }
}

SensorCommunication::~SensorCommunication() = default;
