#include "sensor.h"

#include <QSqlRecord>

Sensor::Sensor(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port)
    : SensorState(uuid, name, capabilities, address, port),
      m_isActive(true),
      m_isReplaced(false),
      m_communicationFailedCount(0),
      m_temperatureThreshold(default_temperature_threshold),
      m_co2ConcentrationThreshold(default_co2_concentration_threshold),
      m_pollutionThreshold(default_pollution_threshold),
      m_isAlarmed(false),
      m_map(nullptr),
      m_x(0),
      m_y(0)
{
    setTemperature(-20);
    setCo2Concentration(0);
    setPollution(0);

    QObject::connect(this, &SensorState::temperatureChanged, this, &Sensor::recalculateAlarmState);
    QObject::connect(this, &SensorState::smokeDetectedChanged, this, &Sensor::recalculateAlarmState);
    QObject::connect(this, &SensorState::co2ConcentrationChanged, this, &Sensor::recalculateAlarmState);
    QObject::connect(this, &SensorState::pollutionChanged, this, &Sensor::recalculateAlarmState);

    QObject::connect(this, &Sensor::temperatureThresholdChanged, this, &Sensor::recalculateAlarmState);
    QObject::connect(this, &Sensor::co2ConcentrationThresholdChanged, this, &Sensor::recalculateAlarmState);
    QObject::connect(this, &Sensor::pollutionThresholdChanged, this, &Sensor::recalculateAlarmState);
}

std::unique_ptr<Sensor> Sensor::fromJson(const QJsonObject& json)
{
    return std::unique_ptr<Sensor>(new Sensor(QUuid::fromString(json["uuid"].toString()),
                                              json["name"].toString(),
                                              Capabilities::fromInt(json["capabilities"].toInt()),
                                              QHostAddress(json["address"].toString()),
                                              json["port"].toInt()));
}

std::unique_ptr<Sensor> Sensor::fromSqlRecord(const QSqlRecord& record)
{
    auto uuid = record.value("uuid").toUuid();
    auto name = record.value("name").toString();
    auto capabilities = Capabilities::fromInt(record.value("capabilities").toInt());
    QHostAddress address{record.value("address").toString()};
    auto port = record.value("port").toInt();

    auto sensor = std::unique_ptr<Sensor>(new Sensor(uuid, name, capabilities, address, port));

    sensor->setTemperatureThreshold(record.value("temperature_threshold").toInt());
    sensor->setCo2ConcentrationThreshold(record.value("co2_threshold").toInt());
    sensor->setPollutionThreshold(record.value("pollution_threshold").toInt());

    sensor->setX(record.value("xPlacement").toInt());
    sensor->setY(record.value("yPlacement").toInt());

    return sensor;
}

bool Sensor::isPlaced() const
{
    return m_map != nullptr;
}

bool Sensor::isActive() const
{
    return m_isActive;
}

bool Sensor::isReplaced() const
{
    return m_isReplaced;
}

void Sensor::setIsReplaced(bool isReplaced)
{
    if (m_isReplaced != isReplaced)
    {
        m_isReplaced = isReplaced;
        emit isReplacedChanged();
    }
}

void Sensor::reportCommunicationSuccess()
{
    m_communicationFailedCount = 0;

    if (!m_isActive)
    {
        m_isActive = true;
        emit isActiveChanged();
    }
}

void Sensor::reportCommunicationFailure()
{
    if (++m_communicationFailedCount == maxCommunicationFailed)
    {
        m_isActive = false;
        emit isActiveChanged();
    }
}

bool Sensor::alarmOn() const
{
    return m_isAlarmed;
}

void Sensor::recalculateAlarmState()
{
    bool newAlarmState = false;

    if (capabilities().testFlag(Capability::Temperature))
        newAlarmState |= temperature() > m_temperatureThreshold;

    if (capabilities().testFlag(Capability::Smoke))
        newAlarmState |= smokeDetected();

    if (capabilities().testFlag(Capability::CO2Concentration))
        newAlarmState |= co2Concentration() > m_co2ConcentrationThreshold;

    if (capabilities().testFlag(Capability::Pollution))
        newAlarmState |= pollution() > m_pollutionThreshold;

    if (m_isAlarmed != newAlarmState) {
        m_isAlarmed = newAlarmState;
        emit alarmStateChanged();
    }
}

short Sensor::temperatureThreshold() const
{
    return m_temperatureThreshold;
}

void Sensor::setTemperatureThreshold(short temperatureThreshold)
{
    if (m_temperatureThreshold == temperatureThreshold)
        return;

    m_temperatureThreshold = temperatureThreshold;
    emit temperatureThresholdChanged();
}

int Sensor::co2ConcentrationThreshold() const
{
    return m_co2ConcentrationThreshold;
}

void Sensor::setCo2ConcentrationThreshold(int co2ConcentrationThreshold)
{
    if (m_co2ConcentrationThreshold == co2ConcentrationThreshold)
        return;

    m_co2ConcentrationThreshold = co2ConcentrationThreshold;
    emit co2ConcentrationThresholdChanged();
}

short Sensor::pollutionThreshold() const
{
    return m_pollutionThreshold;
}

void Sensor::setPollutionThreshold(short pollutionThreshold)
{
    if (m_pollutionThreshold == pollutionThreshold)
        return;

    m_pollutionThreshold = pollutionThreshold;
    emit pollutionThresholdChanged();
}

int Sensor::x() const
{
    return m_x;
}

void Sensor::setX(int x)
{
    m_x = x;
    emit xChanged();
}

int Sensor::y() const
{
    return m_y;
}

void Sensor::setY(int y)
{
    m_y = y;
    emit yChanged();
}

MapEntry* Sensor::map()
{
    return m_map;
}

void Sensor::setMap(MapEntry* map)
{
    if (m_map == map)
        return;

    if (m_map)
        emit removedFromMap();

    bool isPlacedValueChanged = (m_map && !map) || (!m_map && map);

    m_map = map;
    emit mapChanged();

    if (isPlacedValueChanged)
        emit isPlacedChanged();
}
