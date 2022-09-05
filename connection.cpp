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

    server->sendData("CREATE TABLE IF NOT EXISTS users (id INTEGER NOT NULL PRIMARY KEY, login TEXT, password TEXT, name TEXT, surname TEXT, status TEXT);");
}

Connection::~Connection()
{
    delete ui;
}

void Connection::clickSignInButton() {
    if (ui->LoginLineEdit->text().length() == 0) {
        ui->ErrorLabel->setText("The login must contain at least 1 character");
    }
    else {
        QString data = TCPServer::correct(server->sendData("SELECT id FROM users WHERE login = '" + ui->LoginLineEdit->text() + "' AND password = '" + ui->PasswordLineEdit->text() + "';"));
        if (data == "The request was completed successfully") {
            ui->ErrorLabel->setText("There is no such user");
        }
        else {
            id = data.toInt();
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
