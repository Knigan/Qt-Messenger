#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>
#include "tcpserver.h"

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(int ID, QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~ChangePassword();
    bool cancel;

private slots:
    void clickApplyButton();
    void clickCancelButton();

private:
    Ui::ChangePassword *ui;
    TCPServer* server;
    int id;
};

#endif // CHANGEPASSWORD_H
