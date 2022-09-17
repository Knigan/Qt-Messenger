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
    QString Login = ui->LoginLineEdit->text();
    QString Password = ui->PasswordLineEdit->text();
    QString Name = ui->FirstNameLineEdit->text();
    QString Surname = ui->LastNameLineEdit->text();
    if (Login.length() == 0) {
        ui->ErrorLabel->setText("The login must contain at least 1 character");
    }
    else {
        if (Login.contains(',') || Login.contains('(') || Login.contains(')')) {
            ui->ErrorLabel->setText("Login cannot contain '(', ')' and ',' characters");
        }
        else {
            if (Password.contains(',') || Password.contains('(') || Password.contains(')')) {
                ui->ErrorLabel->setText("Password cannot contain '(', ')' and ',' characters");
            }
            else {
                if (Name.contains(',') || Name.contains('(') || Name.contains(')')) {
                    ui->ErrorLabel->setText("The first name cannot contain '(', ')' and ',' characters");
                }
                else {
                    if (Surname.contains(',') || Surname.contains('(') || Surname.contains(')')) {
                        ui->ErrorLabel->setText("The last name cannot contain '(', ')' and ',' characters");
                    }
                    else {
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
