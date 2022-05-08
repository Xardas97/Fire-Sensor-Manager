#include "tcpclient.h"

#include <QDebug>
#include <QTcpSocket>

TcpClient::TcpClient() { }

QByteArray TcpClient::sendRequest(const QHostAddress& address, quint16 port, const char* requestData)
{
    QTcpSocket socket;

    qDebug() << "Client - Connecting to port: " << port;
    socket.connectToHost(address, port);

    if (!socket.waitForConnected(3000))
    {
        qWarning("Client - Failed to connect to server!");
        return QByteArray();
    }

    socket.write(requestData);
    socket.waitForBytesWritten();

    qDebug() << "Client - Waiting to read data";
    if (!socket.waitForReadyRead(3000))
    {
        qWarning() << "Client - No data arrived!";
        socket.close();
        return QByteArray();
    }

    QByteArray data = socket.readAll();
    qDebug() << "Client - Data has arrived: " << data;

    socket.close();

    if (data[0] == '0')
    {
        qWarning() << "Client - Error response received!";
        return QByteArray();
    }

    data.remove(0, 1);

    return data;
}
