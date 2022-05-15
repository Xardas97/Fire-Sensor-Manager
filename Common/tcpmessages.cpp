#include "tcpmessages.h"

namespace TcpMessages
{
    const QJsonObject Command::Identify        = {{"command", "IDENTIFY"}};
    const QJsonObject Command::GetData         = {{"command", "GET_DATA"}};
    const QJsonObject Command::DiscoverSensors = {{"command", "DISCOVER_FIRE_SENSORS"}};

    const QJsonObject Response::Ack                  = {{"response", "ACK"}};
    const QJsonObject Response::BrokenData           = {{"response", "BROKEN_DATA"}};
    const QJsonObject Response::CommunicationFailed  = {{"response", "COMMUNICATION_FAILED"}};
    const QJsonObject Response::CommandNotRecognized = {{"response", "COMMAND_NOT_RECOGNIZED"}};
}
