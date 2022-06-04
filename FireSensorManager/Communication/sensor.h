#ifndef SENSOR_H
#define SENSOR_H

#include "sensorstate.h"

class Sensor : public SensorState
{
    Q_OBJECT

    Q_PROPERTY(bool isActive   READ isActive   NOTIFY isActiveChanged)
    Q_PROPERTY(bool isReplaced READ isReplaced NOTIFY isReplacedChanged)

    private:
        static const short maxCommunicationFailed = 3;

    public:
        static std::unique_ptr<Sensor> fromJson(QJsonObject json);

        bool isActive() const;
        bool isReplaced() const;

        void setIsReplaced();
        void reportCommunicationSuccess();
        void reportCommunicationFailure();

    signals:
        void isActiveChanged();
        void isReplacedChanged();

    private:
        Sensor(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port);

        bool m_isActive;
        bool m_isReplaced;
        short m_communicationFailedCount;
};

#endif // SENSOR_H
