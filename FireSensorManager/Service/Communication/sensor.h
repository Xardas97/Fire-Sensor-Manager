#ifndef SENSOR_H
#define SENSOR_H

#include "sensorstate.h"

class MapEntry;
class QSqlRecord;

class Sensor : public SensorState
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaced   READ isPlaced   NOTIFY isPlacedChanged)
    Q_PROPERTY(bool isActive   READ isActive   NOTIFY isActiveChanged)
    Q_PROPERTY(bool isReplaced READ isReplaced NOTIFY isReplacedChanged)

    Q_PROPERTY(bool alarmOn READ alarmOn NOTIFY alarmStateChanged)

    Q_PROPERTY(short temperatureThreshold      READ temperatureThreshold      WRITE setTemperatureThreshold      NOTIFY temperatureThresholdChanged)
    Q_PROPERTY(int   co2ConcentrationThreshold READ co2ConcentrationThreshold WRITE setCo2ConcentrationThreshold NOTIFY co2ConcentrationThresholdChanged)
    Q_PROPERTY(short pollutionThreshold        READ pollutionThreshold        WRITE setPollutionThreshold        NOTIFY pollutionThresholdChanged)

    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)

    private:
        static const short maxCommunicationFailed = 3;

        static const short default_temperature_threshold     = 30;
        static const int default_co2_concentration_threshold = 1000;
        static const short default_pollution_threshold       = 100;

    public:
        static std::unique_ptr<Sensor> fromJson(const QJsonObject& json);
        static std::unique_ptr<Sensor> fromSqlRecord(const QSqlRecord& record);

        bool isPlaced() const;
        bool isActive() const;
        bool isReplaced() const;

        void setIsReplaced(bool isReplaced);
        void reportCommunicationSuccess();
        void reportCommunicationFailure();

        bool alarmOn() const;

        short temperatureThreshold() const;
        void setTemperatureThreshold(short temperatureThreshold);
        int co2ConcentrationThreshold() const;
        void setCo2ConcentrationThreshold(int co2ConcentrationThreshold);
        short pollutionThreshold() const;
        void setPollutionThreshold(short pollutionThreshold);

        int x() const;
        void setX(int x);
        int y() const;
        void setY(int y);
        MapEntry* map();
        void setMap(MapEntry* map);

    signals:
        bool isPlacedChanged();
        void isActiveChanged();
        void isReplacedChanged();

        void alarmStateChanged();

        void temperatureThresholdChanged();
        void co2ConcentrationThresholdChanged();
        void pollutionThresholdChanged();

        void xChanged();
        void yChanged();

        void mapChanged();
        void removedFromMap();

    private:
        Sensor(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port);

        void recalculateAlarmState();

        bool m_isActive;
        bool m_isReplaced;
        short m_communicationFailedCount;

        short m_temperatureThreshold;
        int m_co2ConcentrationThreshold;
        short m_pollutionThreshold;

        bool m_isAlarmed;

        MapEntry* m_map;
        int m_x, m_y;
};

#endif // SENSOR_H
