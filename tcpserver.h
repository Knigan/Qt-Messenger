#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpSocket>

#define LOCALHOST "192.168.88.254"
#define REMOTEHOST "188.191.162.78"

class TCPServer {
public:
    TCPServer();
    ~TCPServer() {}
    QString sendData(QString);
    static QString correct(QString);

private:
    QTcpSocket* socket;
    QString host;
};

#endif // TCPSERVER_H
