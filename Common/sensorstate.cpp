#include "sensorstate.h"

#include <QRandomGenerator>

SensorState::SensorState(Capabilities capabilities, QHostAddress address, quint16 port)
    : SensorState(QUuid::createUuid(), generateSensorName(capabilities), capabilities, address, port)
{ }

SensorState::SensorState(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port)
    : QObject {nullptr},
      m_uuid(uuid),
      m_name(name),
      m_capabilities(capabilities),
      m_address(address),
      m_port(port),
      m_status(Status::NoIssue),
      m_temperature(20),
      m_smokeDetected(false),
      m_co2Concentration(500),
      m_pollution(30)
{ }

QString SensorState::generateSensorName(Capabilities capabilities)
{
    QString name;

    if (capabilities.testFlag(Capability::Temperature))
        name += "T";
    if (capabilities.testFlag(Capability::Smoke))
        name += "S";
    if (capabilities.testFlag(Capability::CO2Concentration))
        name += "C";
    if (capabilities.testFlag(Capability::Pollution))
        name += "P";

    name += ("-" + QString::number(QRandomGenerator::global()->bounded(1000, 10000)));

    return name;
}

QJsonObject SensorState::toIdentityJson() const
{
    QJsonObject json;

    json["uuid"] = m_uuid.toString(QUuid::StringFormat::WithoutBraces);
    json["name"] = m_name;
    json["capabilities"] = (ushort)m_capabilities.toInt();
    json["address"] = m_address.toString();
    json["port"] = m_port;

    return json;
}

std::unique_ptr<SensorState> SensorState::fromJson(const QJsonObject& json)
{
    return std::unique_ptr<SensorState>(new SensorState(QUuid::fromString(json["uuid"].toString()),
                                                        json["name"].toString(),
                                                        Capabilities::fromInt(json["capabilities"].toInt()),
                                                        QHostAddress(json["address"].toString()),
                                                        json["port"].toInt()));
}

QJsonObject SensorState::toDataJson() const
{
    QJsonObject json;

    json["name"] = m_name;
    json["status"] = (ushort)m_status.toInt();
    // API user needs UUID to ensure that it's still the same sensor on this address
    json["uuid"] = m_uuid.toString(QUuid::StringFormat::WithoutBraces);

    if (m_capabilities.testFlag(Capability::Temperature))
        json["temperature"] = m_temperature;
    if (m_capabilities.testFlag(Capability::Smoke))
        json["smoke"] = m_smokeDetected;
    if (m_capabilities.testFlag(Capability::CO2Concentration))
        json["co2"] = m_co2Concentration;
    if (m_capabilities.testFlag(Capability::Pollution))
        json["pollution"] = m_pollution;

    return json;
}

bool SensorState::updateData(const QJsonObject& json)
{
    auto dataUuid = QUuid::fromString(json["uuid"].toString());
    if (dataUuid != m_uuid)
        return false;

    setName(json["name"].toString());
    setStatus(Statuses::fromInt(json["status"].toInt()));

    if (m_capabilities.testFlag(Capability::Temperature))
    {
        if (json.contains("temperature"))
            setTemperature(json["temperature"].toInt());
        else
            qWarning("API - Temperature expected but missing!");
    }

    if (m_capabilities.testFlag(Capability::Smoke))
    {
        if (json.contains("smoke"))
            setSmokeDetected(json["smoke"].toBool());
        else
            qWarning("API - Smoke detection expected but missing!");
    }

    if (m_capabilities.testFlag(Capability::CO2Concentration))
    {
        if (json.contains("co2"))
            setCo2Concentration(json["co2"].toInt());
        else
            qWarning("API - CO2 Concentration expected but missing!");
    }

    if (m_capabilities.testFlag(Capability::Pollution))
    {
        if (json.contains("pollution"))
            setPollution(json["pollution"].toInt());
        else
            qWarning("API - Pollution expected but missing!");
    }

    return true;
}

const QUuid &SensorState::uuid() const
{
    return m_uuid;
}

const QString &SensorState::name() const
{
    return m_name;
}

void SensorState::setName(const QString &newName)
{
    m_name = newName;
    emit nameChanged();
}

const Capabilities &SensorState::capabilities() const
{
    return m_capabilities;
}

const QHostAddress &SensorState::address() const
{
    return m_address;
}

void SensorState::setAddress(const QHostAddress &newAddress)
{
    m_address = newAddress;
}

const QString SensorState::addressString() const
{
    return m_address.toString();
}

quint16 SensorState::port() const
{
    return m_port;
}

void SensorState::setPort(quint16 newPort)
{
    m_port = newPort;
}

const Statuses &SensorState::status() const
{
    return m_status;
}

void SensorState::setStatus(const Statuses &newStatus)
{
    m_status = newStatus;
    emit statusChanged();
}

short SensorState::temperature() const
{
    return m_temperature;
}

void SensorState::setTemperature(short newTemperature)
{
    m_temperature = newTemperature;
    emit temperatureChanged();
}

bool SensorState::smokeDetected() const
{
    return m_smokeDetected;
}

void SensorState::setSmokeDetected(bool newSmokeDetected)
{
    m_smokeDetected = newSmokeDetected;
    emit smokeDetectedChanged();
}

int SensorState::co2Concentration() const
{
    return m_co2Concentration;
}

void SensorState::setCo2Concentration(int newCo2Concentration)
{
    m_co2Concentration = newCo2Concentration;
    emit co2ConcentrationChanged();
}

short SensorState::pollution() const
{
    return m_pollution;
}

void SensorState::setPollution(short newPollution)
{
    m_pollution = newPollution;
    emit pollutionChanged();
}
