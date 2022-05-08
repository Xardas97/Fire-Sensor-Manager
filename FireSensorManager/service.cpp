#include "service.h"

#include <QDebug>
#include <QTcpSocket>
#include "tcpclient.h"
#include "Communication/firesensordetector.h"

Service::Service(QObject *parent) : QObject{parent} {}

int Service::getNextNumber()
{
    TcpClient tcpClient;

    QHostAddress address = QHostAddress("127.0.0.1");
    quint16 port = 56000;

    auto data = tcpClient.sendRequest(address, port, "GET_NUMBER");
    if (data.length() == 0)
    {
        qWarning() << "TCP Communication failed!";
        return -1;
    }

    int nextNumber = data.toInt();
    qDebug() << "User asked for next number, returing: " << nextNumber;
    return nextNumber;
}

void Service::discoverSensors()
{
    FireSensorDetector fireSensorDetector;
    fireSensorDetector.discoverSensors();
}
