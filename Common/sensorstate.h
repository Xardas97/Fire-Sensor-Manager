#ifndef SENSORSTATE_H
#define SENSORSTATE_H

#include <QUuid>
#include <string>
#include <memory>
#include <QFlags>
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

class SensorState
{
    public:
        SensorState(Capabilities capabilities, QHostAddress address, quint16 port);

        QJsonObject toIdentityJson() const;
        QJsonObject toDataJson() const;
        void updateData(QJsonObject json);
        static std::unique_ptr<SensorState> fromJson(QJsonObject json);

        const QUuid &getUuid() const;
        const std::string &getName() const;
        void setName(const std::string &newName);
        const Capabilities &getCapabilities() const;
        const QHostAddress &getAddress() const;
        quint16 getPort() const;
        const Statuses &getStatus() const;
        void setStatus(const Statuses &newStatus);
        short getTemperature() const;
        void setTemperature(short newTemperature);
        bool getSmokeDetected() const;
        void setSmokeDetected(bool newSmokeDetected);
        int getCo2Concentration() const;
        void setCo2Concentration(int newCo2Concentration);
        short getPollution() const;
        void setPollution(short newPollution);

    private:
        SensorState(QUuid uuid, std::string name, Capabilities capabilities, QHostAddress address, quint16 port);

        QUuid uuid;
        std::string name;
        Capabilities capabilities;

        QHostAddress address;
        quint16 port;

        Statuses status;

        short temperature;
        bool smokeDetected;
        int co2Concentration;
        short pollution;

        static std::string generateSensorName(Capabilities capabilities);
};

QDebug inline operator<<(QDebug out, const SensorState& sensorState)
{
    return out << "[uuid: " << sensorState.getUuid() << ", name: " << sensorState.getName().data() <<
                  ", address" << sensorState.getAddress() << ", port: " << sensorState.getPort() << "]";
}

#endif // SENSORSTATE_H
