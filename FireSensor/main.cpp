#include <QCoreApplication>
#include "server.h"
#include "detectionservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer(4);

    DetectionService detectionService {server.getServerAddress(), server.getServerPort()};

    return a.exec();
}
