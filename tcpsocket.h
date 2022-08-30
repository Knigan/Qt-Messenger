#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

class TCPSocket {
public:
    TCPSocket(QTcpSocket* sock = nullptr);
    ~TCPSocket() {}
    void connectToServer(QString, int);
    void sendData(QString);
private:
    QTcpSocket* socket;
};

#endif // TCPSOCKET_H
