#include "server.h"
#include "detectionservice.h"

#include <memory>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

void setupUi(const QGuiApplication& app, QQmlApplicationEngine& engine, QString mainPage)
{
    const QUrl url("qrc:/" + mainPage);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    auto mainPage = "main.qml";

    Server server;
    bool serverStarted = server.startServer(4);

    std::unique_ptr<DetectionService> detectionService;
    if (serverStarted)
    {
        detectionService = std::make_unique<DetectionService>(server.getServerAddress(), server.getServerPort());
    }
    else
    {
        mainPage = "FailedToStart.qml";
    }

    QQmlApplicationEngine engine;
    setupUi(app, engine, mainPage);
    return app.exec();
}
