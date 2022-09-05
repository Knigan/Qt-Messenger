#include "deletecontact.h"
#include "ui_deletecontact.h"

DeleteContact::DeleteContact(int ID, QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::DeleteContact),
    id(ID)
{
    ui->setupUi(this);

    if (s == nullptr) {
        server = new TCPServer;
    }
    else {
        server = s;
    }

    connect(ui->DeleteButton, &QPushButton::clicked, this, &DeleteContact::clickDeleteButton);
    connect(ui->CancelButton, &QPushButton::clicked, this, &DeleteContact::clickCancelButton);

    cancel = false;
}

DeleteContact::~DeleteContact()
{
    delete ui;
}

void DeleteContact::clickDeleteButton() {
    QString login = ui->LoginLineEdit->text();
    if (login.length() == 0) {
        ui->ErrorLabel->setText("The login must contain at least 1 character");
    }
    else {
        int contact_id = TCPServer::correct(server->sendData("SELECT id FROM users WHERE login = '" + login + "';")).toInt();
        server->sendData("DELETE FROM contacts WHERE user_id = " + QString::number(id) + " AND contact_id = " + QString::number(contact_id) + ";");
        close();
    }
}

void DeleteContact::clickCancelButton() {
    cancel = true;
    close();
}
