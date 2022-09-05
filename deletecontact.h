#ifndef DELETECONTACT_H
#define DELETECONTACT_H

#include <QDialog>
#include "tcpserver.h"

namespace Ui {
class DeleteContact;
}

class DeleteContact : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteContact(int ID, QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~DeleteContact();
    bool cancel;

private slots:
    void clickDeleteButton();
    void clickCancelButton();

private:
    Ui::DeleteContact *ui;
    TCPServer* server;
    int id;
};

#endif // DELETECONTACT_H
