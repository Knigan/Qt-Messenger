#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDialog>
#include "tcpserver.h"
#include "registration.h"

namespace Ui {
class Connection;
}

class Connection : public QDialog
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~Connection();
    int getId() const;

private slots:
    void clickSignInButton();
    void clickSignUpButton();

private:
    Ui::Connection *ui;
    TCPServer* server;
    int id;
};

#endif // CONNECTION_H
