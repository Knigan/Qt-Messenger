#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    connect(ui->SignUpButton, &QPushButton::clicked, this, &Registration::clickSignUpButton);
    connect(ui->CancelButton, &QPushButton::clicked, this, &Registration::clickCancelButton);

    if (s == nullptr) {
        server = new TCPServer;
    }
    else {
        server = s;
    }

    cancel = false;
}

Registration::~Registration()
{
    delete ui;
}

void Registration::clickSignUpButton() {
    bool flag = true;
    QString Login = ui->LoginLineEdit->text();
    QString Password = ui->PasswordLineEdit->text();
    QString Name = ui->FirstNameLineEdit->text();
    QString Surname = ui->LastNameLineEdit->text();
    if (Login.length() == 0) {
        ui->ErrorLabel->setText("The login must contain at least 1 character");
    }
    else {
        if (Login != TCPServer::correct(Login)) {
            ui->ErrorLabel->setText("Login contains restricted characters or excess space symbols");
            flag = false;
        }
        if (Password != TCPServer::correct(Password)) {
            ui->ErrorLabel->setText("Password contains restricted characters or excess space symbols");
            flag = false;
        }
        if (Name != TCPServer::correct(Name)) {
            ui->ErrorLabel->setText("Name contains restricted characters or excess space symbols");
            flag = false;
        }
        if (Surname != TCPServer::correct(Surname)) {
            ui->ErrorLabel->setText("Surname contains restricted characters or excess space symbols");
            flag = false;
        }
        if (flag) {
            QString check = server->sendData("SELECT id FROM users WHERE login = '" + ui->LoginLineEdit->text() + "';");
            if (check == "The request was completed successfully") {
                int count = TCPServer::correct(server->sendData("SELECT MAX(id) FROM users;")).toInt();
                server->sendData("INSERT INTO users VALUES (" + QString::number(count + 1) + ", '" + ui->LoginLineEdit->text() + "', '"
                                 + ui->PasswordLineEdit->text() + "', '" + ui->FirstNameLineEdit->text() + "', '" + ui->LastNameLineEdit->text() + "', '');");
                close();
            }
            else {
                ui->ErrorLabel->setText("The entered login is already used");
            }
        }
    }
}

void Registration::clickCancelButton() {
    cancel = true;
    close();
}

QString Registration::getLogin() const {
    return ui->LoginLineEdit->text();
}

QString Registration::getPassword() const {
    return ui->PasswordLineEdit->text();
}
