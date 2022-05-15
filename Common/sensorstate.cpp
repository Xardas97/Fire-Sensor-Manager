#include "sensorstate.h"

#include <QRandomGenerator>

SensorState::SensorState(Capabilities capabilities, QHostAddress address, quint16 port)
    : SensorState(QUuid::createUuid(), generateSensorName(capabilities), capabilities, address, port)
{ }

SensorState::SensorState(QUuid uuid, std::string name, Capabilities capabilities, QHostAddress address, quint16 port)
    : uuid(uuid),
      name(name),
      capabilities(capabilities),
      address(address),
      port(port),
      status(Status::NoIssue),
      temperature(20),
      smokeDetected(false),
      co2Concentration(500),
      pollution(30)
{ }

std::string SensorState::generateSensorName(Capabilities capabilities)
{
    std::string name;

    if (capabilities.testFlag(Capability::Temperature))
        name += "T";
    if (capabilities.testFlag(Capability::Smoke))
        name += "S";
    if (capabilities.testFlag(Capability::CO2Concentration))
        name += "C";
    if (capabilities.testFlag(Capability::Pollution))
        name += "P";

    name += ("-" + std::to_string(QRandomGenerator::global()->bounded(1000, 10000)));

    return name;
}

QJsonObject SensorState::toIdentityJson() const
{
    QJsonObject json;

    json["uuid"] = uuid.toString(QUuid::StringFormat::WithoutBraces);
    json["name"] = name.c_str();
    json["capabilities"] = QString::number(capabilities.toInt());
    json["address"] = address.toString();
    json["port"] = port;

    return json;
}

SensorState SensorState::fromJson(QJsonObject json)
{
    return SensorState(QUuid::fromString(json["uuid"].toString()),
                       json["name"].toString().toStdString(),
                       Capabilities::fromInt(json["capabilities"].toInt()),
                       QHostAddress(json["address"].toString()),
                       json["port"].toInt());
}

QJsonObject SensorState::toDataJson() const
{
    QJsonObject json;

    json["name"] = name.c_str();
    json["status"] = QString::number(status.toInt());

    if (capabilities.testFlag(Capability::Temperature))
        json["temperature"] = temperature;
    if (capabilities.testFlag(Capability::Smoke))
        json["smoke"] = smokeDetected;
    if (capabilities.testFlag(Capability::CO2Concentration))
        json["co2"] = co2Concentration;
    if (capabilities.testFlag(Capability::Pollution))
        json["pollution"] = pollution;

    return json;
}

void SensorState::updateData(QJsonObject json)
{
    if (capabilities.testFlag(Capability::Temperature))
    {
        if (json.contains("temperature"))
            temperature = json["temperature"].toInt();
        else
            qWarning("API - Temperature expected but missing!");
    }

    if (capabilities.testFlag(Capability::Smoke))
    {
        if (json.contains("smoke"))
            smokeDetected = json["smoke"].toBool();
        else
            qWarning("API - Smoke detection expected but missing!");
    }

    if (capabilities.testFlag(Capability::CO2Concentration))
    {
        if (json.contains("co2"))
            co2Concentration =json["co2"].toInt();
        else
            qWarning("API - CO2 Concentration expected but missing!");
    }

    if (capabilities.testFlag(Capability::Pollution))
    {
        if (json.contains("pollution"))
            pollution = json["pollution"].toInt();
        else
            qWarning("API - Pollution expected but missing!");
    }
}

const QUuid &SensorState::getUuid() const
{
    return uuid;
}

const std::string &SensorState::getName() const
{
    return name;
}

void SensorState::setName(const std::string &newName)
{
    name = newName;
}

const Capabilities &SensorState::getCapabilities() const
{
    return capabilities;
}

const QHostAddress &SensorState::getAddress() const
{
    return address;
}

quint16 SensorState::getPort() const
{
    return port;
}

const Statuses &SensorState::getStatus() const
{
    return status;
}

void SensorState::setStatus(const Statuses &newStatus)
{
    status = newStatus;
}

short SensorState::getTemperature() const
{
    return temperature;
}

void SensorState::setTemperature(short newTemperature)
{
    temperature = newTemperature;
}

bool SensorState::getSmokeDetected() const
{
    return smokeDetected;
}

void SensorState::setSmokeDetected(bool newSmokeDetected)
{
    smokeDetected = newSmokeDetected;
}

int SensorState::getCo2Concentration() const
{
    return co2Concentration;
}

void SensorState::setCo2Concentration(int newCo2Concentration)
{
    co2Concentration = newCo2Concentration;
}

short SensorState::getPollution() const
{
    return pollution;
}

void SensorState::setPollution(short newPollution)
{
    pollution = newPollution;
}
