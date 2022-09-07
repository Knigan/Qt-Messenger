#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);
    connect(ui->SignInButton, &QPushButton::clicked, this, &Connection::clickSignInButton);
    connect(ui->SignUpButton, &QPushButton::clicked, this, &Connection::clickSignUpButton);

    if (s == nullptr) {
        server = new TCPServer;
    }
    else {
        server = s;
    }

    connection_settings = new QSettings("connection_config.ini", QSettings::IniFormat, this);
    ui->LoginLineEdit->setText(connection_settings->value("Login").toString());
}

Connection::~Connection() {
    delete ui;
}

void Connection::clickSignInButton() {
    QString login = ui->LoginLineEdit->text();
    if (login.length() == 0) {
        ui->ErrorLabel->setText("The login must contain at least 1 character");
    }
    else {
        QString data = TCPServer::correct(server->sendData("SELECT id FROM users WHERE login = '" + login + "' AND password = '" + ui->PasswordLineEdit->text() + "';"));
        if (data == "The request was completed successfully") {
            ui->ErrorLabel->setText("There is no such user");
        }
        else {
            id = data.toInt();
            connection_settings->setValue("Login", login);
            close();
        }
    }
}

void Connection::clickSignUpButton() {
    Registration r(this, server);
    r.exec();

    if (!r.cancel) {
        ui->LoginLineEdit->setText(r.getLogin());
        ui->PasswordLineEdit->setText(r.getPassword());
    }
}

int Connection::getId() const {
    return id;
}
