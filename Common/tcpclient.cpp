#include "tcpclient.h"

#include "tcpmessages.h"

#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>

TcpClient::TcpClient() { }

QJsonObject TcpClient::sendRequest(const QHostAddress& address, quint16 port, const QJsonObject& requestData)
{
    QTcpSocket socket;

    socket.connectToHost(address, port);

    if (!socket.waitForConnected(500))
    {
        return TcpMessages::Response::CommunicationFailed;
    }

    socket.write(TcpMessages::getBytes(requestData));
    socket.waitForBytesWritten();

    if (!socket.waitForReadyRead(500))
    {
        socket.close();
        return TcpMessages::Response::CommunicationFailed;
    }

    QByteArray data = socket.readAll();

    socket.close();

    QJsonDocument dataDoc = QJsonDocument::fromJson(data);
    return dataDoc.object();
}
