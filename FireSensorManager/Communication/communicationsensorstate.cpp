#include "communicationsensorstate.h"


CommunicationSensorState::CommunicationSensorState(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port)
    : SensorState(uuid, name, capabilities, address, port),
      isActive(true),
      isReplaced(false)
{ }

std::unique_ptr<CommunicationSensorState> CommunicationSensorState::fromJson(QJsonObject json)
{
    return std::unique_ptr<CommunicationSensorState>(new CommunicationSensorState(QUuid::fromString(json["uuid"].toString()),
                                                                                  json["name"].toString(),
                                                                                  Capabilities::fromInt(json["capabilities"].toInt()),
                                                                                  QHostAddress(json["address"].toString()),
                                                                                  json["port"].toInt()));
}

bool CommunicationSensorState::getIsActive() const
{
    return isActive;
}

bool CommunicationSensorState::getIsReplaced() const
{
    return isReplaced;
}

void CommunicationSensorState::setIsReplaced()
{
    if (!isReplaced)
    {
        isReplaced = true;
        emit isReplacedChanged();
    }
}

void CommunicationSensorState::reportCommunicationSuccess()
{
    communicationFailedCount = 0;

    if (!isActive)
    {
        qDebug() << "Communication with sensor " << getName() << " succeeded, setting as active again!";
        isActive = true;
        emit isActiveChanged();
    }
}

void CommunicationSensorState::reportCommunicationFailure()
{
    if (++communicationFailedCount == maxCommunicationFailed)
    {
        qDebug() << "Communication with sensor " << getName() << " failed " << maxCommunicationFailed << " times, setting as inactive!";
        isActive = false;
        emit isActiveChanged();
    }
}
