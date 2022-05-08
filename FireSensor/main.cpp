#include <QCoreApplication>
#include "server.h"
#include "detectionservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DetectionService detectionService;

    Server server;
    server.startServer(4);

    return a.exec();
}
