#include "tcpserver.h"

TCPServer::TCPServer() :
    host(LOCALHOST)
{
    socket = new QTcpSocket;
}

QString TCPServer::sendData(QString wData) {
    socket->connectToHost(host, 31416);
    socket->write(wData.toUtf8());
    socket->waitForReadyRead(3000);
    QString rData = QString(socket->readAll());
    socket->close();
    return rData;
}
