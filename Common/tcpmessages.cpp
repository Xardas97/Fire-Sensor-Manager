#include "tcpmessages.h"

namespace TcpMessages
{
    const char* Command::GetNumber       = "GET_NUMBER";
    const char* Command::DiscoverSensors = "DISCOVER_FIRE_SENSORS";
    const char* Command::Identify        = "IDENTIFY";

    const char* Reply::Ack                  = "ACK";
    const char* Reply::BrokenData           = "BROKEN_DATA";
    const char* Reply::CommandNotRecognized = "COMMAND_NOT_RECOGNIZED";
}
