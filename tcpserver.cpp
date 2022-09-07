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

QString TCPServer::correct(QString str) {
    str.remove(QChar('('));
    str.remove(QChar(')'));
    str.replace(QString("'"), QString(""));
    str.remove(QChar('"'));
    str.remove(QChar(','));
    str.remove(QChar('\n'));

    while (str[0] == ' ') {
        str.remove(0, 1);
    }
    while (str[str.length() - 1] == ' ') {
        str.remove(str.length() - 1, 1);
    }

    return str;
}
