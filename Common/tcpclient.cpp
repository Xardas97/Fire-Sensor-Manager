#include "tcpclient.h"

#include "tcpmessages.h"

#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>

TcpClient::TcpClient() { }

QJsonObject TcpClient::sendRequest(const QHostAddress& address, quint16 port, const QJsonObject& requestData)
{
    QTcpSocket socket;

    qDebug() << "Client - Connecting to port: " << port;
    socket.connectToHost(address, port);

    if (!socket.waitForConnected(3000))
    {
        qWarning("Client - Failed to connect to server!");
        return TcpMessages::Response::CommunicationFailed;
    }

    socket.write(TcpMessages::getBytes(requestData));
    socket.waitForBytesWritten();

    qDebug() << "Client - Waiting to read data";
    if (!socket.waitForReadyRead(3000))
    {
        qWarning() << "Client - No data arrived!";
        socket.close();
        return TcpMessages::Response::CommunicationFailed;
    }

    QByteArray data = socket.readAll();
    qDebug() << "Client - Data has arrived: " << data;

    socket.close();

    QJsonDocument dataDoc = QJsonDocument::fromJson(data);
    return dataDoc.object();
}
