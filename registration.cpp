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
    if (ui->LoginLineEdit->text().length() == 0) {
        ui->ErrorLabel->setText("The login must contain at least 1 character");
    }
    else {
        QString check = server->sendData("SELECT id FROM users WHERE login = '" + ui->LoginLineEdit->text() + "';");
        if (check == "The request was completed successfully") {
            int count = server->sendData("SELECT COUNT(id) FROM users;").toInt();
            server->sendData("INSERT INTO users VALUES (" + QString::number(count + 1) + ", '" + ui->LoginLineEdit->text() + "', '"
                             + ui->PasswordLineEdit->text() + "', '" + ui->FirstNameLineEdit->text() + "', '" + ui->LastNameLineEdit->text() + "', '');");
            close();
        }
        else {
            ui->ErrorLabel->setText("The entered login is already used");
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
