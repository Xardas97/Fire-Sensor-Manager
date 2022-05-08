#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "service.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Service service;

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("service", &service);

    return app.exec();
}
