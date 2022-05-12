#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "server.h"
#include "detectionservice.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Server server;
    server.startServer(4);

    DetectionService detectionService {server.getServerAddress(), server.getServerPort()};

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
