#include "tcpserver.h"

TCPServer::TCPServer() :
    host(LOCALHOST)
{
    socket = new QTcpSocket;
}

QString TCPServer::sendData(QString data) {
    socket->connectToHost(host, 31416);
    socket->write(data.toUtf8());
    QString readData = QString::fromUtf8(socket->read(1024));
    socket->close();
    return readData;
}
