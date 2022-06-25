#include <QQmlContext>
#include <QGuiApplication>
#include <QQuickImageProvider>
#include <QQmlApplicationEngine>

#include "service.h"
#include "database.h"
#include "Map/mapimageprovider.h"
#include "Communication/sensor.h"

void setupEngineWithStartingPage(QQmlApplicationEngine& engine, const QGuiApplication& app, std::string startingPage)
{
    const QUrl url(startingPage.c_str());
    QObject::connect(&engine,
                     &QQmlApplicationEngine::objectCreated,
                     &app,
                     [url](QObject *obj, const QUrl &objUrl)
                     {
                        if (!obj && url == objUrl)
                            QCoreApplication::exit(-1);
                     },
                     Qt::QueuedConnection);

    engine.load(url);
}

void setupEngineWithService(QQmlApplicationEngine& engine, Service& service)
{
    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("service", &service);

    engine.addImageProvider("MapImageProvider", service.createMapImageProvider());
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<Sensor>("Custom.Sensors", 1, 0, "Sensor", "Sensor QML object cannot be created");

    try
    {
        Service service;

        QQmlApplicationEngine engine;
        setupEngineWithService(engine, service);
        setupEngineWithStartingPage(engine, app, "qrc:/Interface/main.qml");

        return app.exec();
    }
    catch (FailedToUseDatabaseException ex)
    {
        qWarning() << "Failed to use the database!";

        QQmlApplicationEngine engine;
        setupEngineWithStartingPage(engine, app, "qrc:/Interface/FailedToStart.qml");

        return app.exec();
    }
}
