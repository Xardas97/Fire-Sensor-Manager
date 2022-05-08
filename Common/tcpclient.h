#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QHostAddress>

class TcpClient
{
public:
    TcpClient();
    QByteArray sendRequest(const QHostAddress& address, quint16 port, const char* requestData);
};

#endif // TCPCLIENT_H
