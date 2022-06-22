#include "sensor.h"
#include "detectionservice.h"

#include <memory>
#include <QQmlContext>
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

    Sensor sensor;

    QQmlApplicationEngine engine;

    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("sensor", &sensor);

    setupUi(app, engine, "Interface/main.qml");

    return app.exec();
}
