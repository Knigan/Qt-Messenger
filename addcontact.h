#ifndef ADDCONTACT_H
#define ADDCONTACT_H

#include <QDialog>
#include "tcpserver.h"

namespace Ui {
class AddContact;
}

class AddContact : public QDialog
{
    Q_OBJECT

public:
    explicit AddContact(int ID, QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~AddContact();
    bool cancel;

private slots:
    void clickAddButton();
    void clickCancelButton();

private:
    Ui::AddContact *ui;
    TCPServer* server;
    int id;
};

#endif // ADDCONTACT_H
