#include "addcontact.h"
#include "ui_addcontact.h"

AddContact::AddContact(int ID, QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::AddContact),
    id(ID)
{
    ui->setupUi(this);

    if (s == nullptr) {
        server = new TCPServer;
    }
    else {
        server = s;
    }

    connect(ui->AddButton, &QPushButton::clicked, this, &AddContact::clickAddButton);
    connect(ui->CancelButton, &QPushButton::clicked, this, &AddContact::clickCancelButton);

    cancel = false;
}

AddContact::~AddContact()
{
    delete ui;
}

void AddContact::clickAddButton() {
    QString login = ui->LoginLineEdit->text();
    if (login.length() == 0) {
        ui->ErrorLabel->setText("The login must contain at least 1 character");
    }
    else {
        int count = TCPServer::correct(server->sendData("SELECT MAX(id) FROM contacts;")).toInt();
        QString str = TCPServer::correct(server->sendData("SELECT id FROM users WHERE login = '" + login + "';"));
        if (str == "The request was completed successfully") {
            ui->ErrorLabel->setText("There is no such user");
        }
        else {
            int contact_id = str.toInt();
            int check = TCPServer::correct(server->sendData("SELECT COUNT(*) FROM contacts WHERE user_id = " + QString::number(id) + " AND contact_id = " + QString::number(contact_id))).toInt();
            if (check == 0) {
                server->sendData("INSERT INTO contacts VALUES (" + QString::number(count + 1) + ", " + QString::number(id) + ", " + QString::number(contact_id) + ");");
                close();
            }
            else {
                ui->ErrorLabel->setText("This person is already your contact");
            }
        }
    }
}

void AddContact::clickCancelButton() {
    cancel = true;
    close();
}
