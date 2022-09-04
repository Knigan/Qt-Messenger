#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include "tcpserver.h"

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~Registration();
    QString getLogin() const;
    QString getPassword() const;
    bool cancel;

private slots:
    void clickSignUpButton();
    void clickCancelButton();

private:
    Ui::Registration *ui;
    TCPServer* server;
};

#endif // REGISTRATION_H
