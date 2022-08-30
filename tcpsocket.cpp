#include "tcpsocket.h"

TCPSocket::TCPSocket(QTcpSocket* sock)
{
    if (sock == nullptr) {
        socket = new QTcpSocket;
    }
    else {
        socket = new QTcpSocket(sock);
    }
}

void TCPSocket::connectToServer(QString host, int port) {
    socket->connectToHost(host, port);
}

void TCPSocket::sendData(QString str) {
    QByteArray data;
    QDataStream out(&data, QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    socket->write(data);
}
