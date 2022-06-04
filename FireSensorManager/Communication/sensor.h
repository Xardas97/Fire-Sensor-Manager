#ifndef SENSOR_H
#define SENSOR_H

#include "sensorstate.h"

class Sensor : public SensorState
{
    Q_OBJECT

    Q_PROPERTY(bool isActive   READ isActive   NOTIFY isActiveChanged)
    Q_PROPERTY(bool isReplaced READ isReplaced NOTIFY isReplacedChanged)

    Q_PROPERTY(bool alarmOn READ alarmOn NOTIFY alarmStateChanged)

    Q_PROPERTY(short temperatureThreshold      READ temperatureThreshold      WRITE setTemperatureThreshold      NOTIFY temperatureThresholdChanged)
    Q_PROPERTY(int   co2ConcentrationThreshold READ co2ConcentrationThreshold WRITE setCo2ConcentrationThreshold NOTIFY co2ConcentrationThresholdChanged)
    Q_PROPERTY(short pollutionThreshold        READ pollutionThreshold        WRITE setPollutionThreshold        NOTIFY pollutionThresholdChanged)

    private:
        static const short maxCommunicationFailed = 3;

        static const short default_temperature_threshold     = 30;
        static const int default_co2_concentration_threshold = 1000;
        static const short default_pollution_threshold       = 100;

    public:
        static std::unique_ptr<Sensor> fromJson(QJsonObject json);

        bool isActive() const;
        bool isReplaced() const;

        void setIsReplaced();
        void reportCommunicationSuccess();
        void reportCommunicationFailure();

        bool alarmOn() const;

        short temperatureThreshold() const;
        void setTemperatureThreshold(short temperatureThreshold);
        int co2ConcentrationThreshold() const;
        void setCo2ConcentrationThreshold(int co2ConcentrationThreshold);
        short pollutionThreshold() const;
        void setPollutionThreshold(short pollutionThreshold);

    signals:
        void isActiveChanged();
        void isReplacedChanged();

        void alarmStateChanged();

        void temperatureThresholdChanged();
        void co2ConcentrationThresholdChanged();
        void pollutionThresholdChanged();

    private:
        Sensor(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port);

        bool m_isActive;
        bool m_isReplaced;
        short m_communicationFailedCount;

        short m_temperatureThreshold;
        int m_co2ConcentrationThreshold;
        short m_pollutionThreshold;
};

#endif // SENSOR_H
