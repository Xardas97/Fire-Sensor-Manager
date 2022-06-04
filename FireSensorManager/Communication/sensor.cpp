#include "sensor.h"

Sensor::Sensor(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port)
    : SensorState(uuid, name, capabilities, address, port),
      isActive(true),
      isReplaced(false)
{ }

std::unique_ptr<Sensor> Sensor::fromJson(QJsonObject json)
{
    return std::unique_ptr<Sensor>(new Sensor(QUuid::fromString(json["uuid"].toString()),
                                                                                  json["name"].toString(),
                                                                                  Capabilities::fromInt(json["capabilities"].toInt()),
                                                                                  QHostAddress(json["address"].toString()),
                                                                                  json["port"].toInt()));
}

bool Sensor::getIsActive() const
{
    return isActive;
}

bool Sensor::getIsReplaced() const
{
    return isReplaced;
}

void Sensor::setIsReplaced()
{
    if (!isReplaced)
    {
        isReplaced = true;
        emit isReplacedChanged();
    }
}

void Sensor::reportCommunicationSuccess()
{
    communicationFailedCount = 0;

    if (!isActive)
    {
        qDebug() << "Communication with sensor " << getName() << " succeeded, setting as active again!";
        isActive = true;
        emit isActiveChanged();
    }
}

void Sensor::reportCommunicationFailure()
{
    if (++communicationFailedCount == maxCommunicationFailed)
    {
        qDebug() << "Communication with sensor " << getName() << " failed " << maxCommunicationFailed << " times, setting as inactive!";
        isActive = false;
        emit isActiveChanged();
    }
}
