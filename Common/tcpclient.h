#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <QHostAddress>

class TcpClient
{
public:
    TcpClient();
    QByteArray sendRequest(const QHostAddress& address, quint16 port, const std::string& requestData);
};

#endif // TCPCLIENT_H
