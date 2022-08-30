#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDialog>
#include "tcpsocket.h"

namespace Ui {
class Connection;
}

class Connection : public QDialog
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = nullptr, QTcpSocket* sock = nullptr);
    ~Connection();
    bool exit;

private slots:
    void clickSendButton();
    void clickExitButton();
    void clickSignUpButton();

private:
    Ui::Connection *ui;
    QTcpSocket* socket;
};

#endif // CONNECTION_H
