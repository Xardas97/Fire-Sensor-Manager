#ifndef TCPMESSAGES_H
#define TCPMESSAGES_H

namespace TcpMessages
{
    class Command
    {
        public:
            static const char* GetNumber;
            static const char* DiscoverSensors;
            static const char* Identify;
    };

    class Reply
    {
        public:
            static const char* Ack;
            static const char* BrokenData;
            static const char* CommandNotRecognized;
    };
}

#endif // TCPMESSAGES_H
