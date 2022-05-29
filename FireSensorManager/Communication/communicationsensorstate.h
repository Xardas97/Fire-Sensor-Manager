#ifndef COMMUNICATIONSENSORSTATE_H
#define COMMUNICATIONSENSORSTATE_H

#include "sensorstate.h"

class CommunicationSensorState;
using Sensor = CommunicationSensorState;

class CommunicationSensorState : public SensorState
{
    Q_OBJECT

    Q_PROPERTY(bool isActive    READ getIsActive    NOTIFY isActiveChanged)
    Q_PROPERTY(bool isReplaced  READ getIsReplaced  NOTIFY isReplacedChanged)

    private:
        static const short maxCommunicationFailed = 3;

    public:
        static std::unique_ptr<CommunicationSensorState> fromJson(QJsonObject json);

        bool getIsActive() const;
        bool getIsReplaced() const;

        void setIsReplaced();
        void reportCommunicationSuccess();
        void reportCommunicationFailure();

    signals:
        void isActiveChanged();
        void isReplacedChanged();

    private:
        CommunicationSensorState(QUuid uuid, QString name, Capabilities capabilities, QHostAddress address, quint16 port);

        bool isActive;
        bool isReplaced;
        short communicationFailedCount;
};

#endif // COMMUNICATIONSENSORSTATE_H
