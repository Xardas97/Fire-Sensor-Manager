#include "service.h"

#include <QDebug>
#include <QTcpSocket>
#include "Communication/firesensordetector.h"

Service::Service(QObject *parent) : QObject{parent} {}

int Service::getNextNumber()
{
    QTcpSocket socket;
    auto port = 56000;

    qDebug() << "Client - Connecting to port: " << port;
    socket.connectToHost("localhost", port);

    if (!socket.waitForConnected(3000))
    {
        qWarning("Client - Failed to connect to server!");
        return -1;
    }

    socket.write("GET_NUMBER");
    socket.waitForBytesWritten();

    qDebug() << "Client - Waiting to read data";
    if (!socket.waitForReadyRead(3000))
    {
        qWarning() << "Client - No data arrived!";
        return -1;
    }

    int nextNumber = 0;

    QString data = socket.readAll();
    qDebug() << "Client - Data has arrived: " << data;

    if (data[0] == '0')
    {
        qWarning() << "Client - Error response received!";
        socket.close();
        return -1;
    }

    data.remove(0, 1);
    nextNumber = data.toInt();

    socket.close();

    qDebug() << "User asked for next number, returing: " << nextNumber;
    return nextNumber;
}

void Service::discoverSensors()
{
    FireSensorDetector fireSensorDetector;
    fireSensorDetector.discoverSensors();
}
