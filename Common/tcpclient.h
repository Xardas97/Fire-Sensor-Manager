#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <QJsonObject>
#include <QHostAddress>

class TcpClient
{
public:
    TcpClient();
    QJsonObject sendRequest(const QHostAddress& address, quint16 port, const QJsonObject& requestData);
};

#endif // TCPCLIENT_H
