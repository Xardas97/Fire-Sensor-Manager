#ifndef SENSORSTATE_H
#define SENSORSTATE_H

#include <QUuid>
#include <string>
#include <memory>
#include <QFlags>
#include <QObject>
#include <QJsonObject>
#include <QHostAddress>

enum Capability
{
    Temperature = 0x1,
    Smoke = 0x2,
    CO2Concentration = 0x4,
    Pollution = 0x8
};

Q_DECLARE_FLAGS(Capabilities, Capability)
Q_DECLARE_OPERATORS_FOR_FLAGS(Capabilities)

enum Status
{
    NoIssue = 0x0,
    Error = 0x1,
    Dirty = 0x2,
    MaintenanceRequired = 0x4
};

Q_DECLARE_FLAGS(Statuses, Status)
Q_DECLARE_OPERATORS_FOR_FLAGS(Statuses)

class SensorState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUuid        uuid             READ uuid           CONSTANT)
    Q_PROPERTY(Capabilities capabilities     READ capabilities   CONSTANT)
    Q_PROPERTY(QString      address          READ addressString  CONSTANT)
    Q_PROPERTY(quint16      port             READ port           CONSTANT)

    Q_PROPERTY(QString      name             READ name             WRITE setName             NOTIFY nameChanged)
    Q_PROPERTY(Statuses     status           READ status           WRITE setStatus           NOTIFY statusChanged)
    Q_PROPERTY(short        temperature      READ temperature      WRITE setTemperature      NOTIFY temperatureChanged)
    Q_PROPERTY(bool         smokeDetected    READ smokeDetected    WRITE setSmokeDetected    NOTIFY smokeDetectedChanged)
    Q_PROPERTY(int          co2Concentration READ co2Concentration WRITE setCo2Concentration NOTIFY co2ConcentrationChanged)
    Q_PROPERTY(short        pollution        READ pollution        WRITE setPollution        NOTIFY pollutionChanged)

    protected:
        static const short default_temperature     = 20;
        static const int default_co2_concentration = 500;
        static const short default_pollution       = 30;

    public:
        SensorState(Capabilities capabilities, QHostAddress address, quint16 port);

        QJsonObject toIdentityJson() const;
        QJsonObject toDataJson() const;
        bool updateData(const QJsonObject& json);
        static std::unique_ptr<SensorState> fromJson(const QJsonObject& json);

        const QUuid &uuid() const;
        const QString &name() const;
        void setName(const QString &newName);
        const Capabilities &capabilities() const;
        const QHostAddress &address() const;
        void setAddress(const QHostAddress &newAddress);
        const QString addressString() const;
        quint16 port() const;
        void setPort(quint16 newPort);
        const Statuses &status() const;
        void setStatus(const Statuses &newStatus);
        short temperature() const;
        void setTemperature(short newTemperature);
        bool smokeDetected() const;
        void setSmokeDetected(bool newSmokeDetected);
        int co2Concentration() const;
        void setCo2Concentration(int newCo2Concentration);
        short pollution() const;
        void setPollution(short newPollution);

    signals:
        void nameChanged();
        void statusChanged();
        void temperatureChanged();
        void smokeDetectedChanged();
        void co2ConcentrationChanged();
        void pollutionChanged();

    protected:
        SensorState(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port);

    private:
        QUuid m_uuid;
        QString m_name;
        Capabilities m_capabilities;

        QHostAddress m_address;
        quint16 m_port;

        Statuses m_status;

        short m_temperature;
        bool m_smokeDetected;
        int m_co2Concentration;
        short m_pollution;

        static QString generateSensorName(Capabilities capabilities);
};

QDebug inline operator<<(QDebug out, const SensorState& sensorState)
{
    return out << "[uuid: " << sensorState.uuid() << ", name: " << sensorState.name() <<
                  ", address" << sensorState.address() << ", port: " << sensorState.port() << "]";
}

#endif // SENSORSTATE_H
