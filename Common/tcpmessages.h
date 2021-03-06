#ifndef TCPMESSAGES_H
#define TCPMESSAGES_H

#include <QJsonObject>
#include <QJsonDocument>

namespace TcpMessages
{
    inline QByteArray getBytes(const QJsonObject& object, QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Compact)
    {
        QJsonDocument doc(object);
        return doc.toJson(format);
    }

    class Command
    {
        public:
            static const QJsonObject Identify;
            static const QJsonObject GetData;
            static const QJsonObject SetName;
            static const QJsonObject DiscoverSensors;
    };

    class Response
    {
        public:
            static const QJsonObject Ack;
            static const QJsonObject BrokenData;
            static const QJsonObject WrongSensor;
            static const QJsonObject CommunicationFailed;
            static const QJsonObject CommandNotRecognized;
    };
}

#endif // TCPMESSAGES_H
