#include "sensor.h"

Sensor::Sensor(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port)
    : SensorState(uuid, name, capabilities, address, port),
      m_isActive(true),
      m_isReplaced(false)
{ }

std::unique_ptr<Sensor> Sensor::fromJson(QJsonObject json)
{
    return std::unique_ptr<Sensor>(new Sensor(QUuid::fromString(json["uuid"].toString()),
                                                                                  json["name"].toString(),
                                                                                  Capabilities::fromInt(json["capabilities"].toInt()),
                                                                                  QHostAddress(json["address"].toString()),
                                                                                  json["port"].toInt()));
}

bool Sensor::isActive() const
{
    return m_isActive;
}

bool Sensor::isReplaced() const
{
    return m_isReplaced;
}

void Sensor::setIsReplaced()
{
    if (!m_isReplaced)
    {
        m_isReplaced = true;
        emit isReplacedChanged();
    }
}

void Sensor::reportCommunicationSuccess()
{
    m_communicationFailedCount = 0;

    if (!m_isActive)
    {
        qDebug() << "Communication with sensor " << name() << " succeeded, setting as active again!";
        m_isActive = true;
        emit isActiveChanged();
    }
}

void Sensor::reportCommunicationFailure()
{
    if (++m_communicationFailedCount == maxCommunicationFailed)
    {
        qDebug() << "Communication with sensor " << name() << " failed " << maxCommunicationFailed << " times, setting as inactive!";
        m_isActive = false;
        emit isActiveChanged();
    }
}
