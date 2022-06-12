#include <QQmlContext>
#include <QGuiApplication>
#include <QQuickImageProvider>
#include <QQmlApplicationEngine>

#include "service.h"
#include "Map/mapimageprovider.h"
#include "Communication/sensor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<Sensor>("Custom.Sensors", 1, 0, "Sensor", "Sensor QML object cannot be created");

    Service service;

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("service", &service);

    MapImageProvider mapImageProvider;
    engine.addImageProvider("MapImageProvider", &mapImageProvider);

    engine.load(url);

    return app.exec();
}
