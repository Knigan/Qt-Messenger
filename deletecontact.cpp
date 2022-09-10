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

    ui->LoginComboBox->clear();

    QString data = server->sendData("SELECT users.login FROM users JOIN contacts ON users.id = contacts.contact_id WHERE contacts.user_id = " + QString::number(id) + ";");
    for (int i = 0; i < data.count("[?~?]"); ++i) {
        ui->LoginComboBox->addItem(TCPServer::correct(data.section("[?~?]", i, i)));
    }

    cancel = false;
}

DeleteContact::~DeleteContact()
{
    delete ui;
}

void DeleteContact::clickDeleteButton() {
    QString login = ui->LoginComboBox->currentText();
    int contact_id = TCPServer::correct(server->sendData("SELECT id FROM users WHERE login = '" + login + "';")).toInt();
    server->sendData("DELETE FROM contacts WHERE user_id = " + QString::number(id) + " AND contact_id = " + QString::number(contact_id) + ";");
    close();
}

void DeleteContact::clickCancelButton() {
    cancel = true;
    close();
}
