#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent, QTcpSocket* sock) :
    QDialog(parent),
    ui(new Ui::Connection),
    socket(sock)
{
    ui->setupUi(this);
    connect(ui->SendButton, &QPushButton::clicked, this, &Connection::clickSendButton);
    connect(ui->ExitButton, &QPushButton::clicked, this, &Connection::clickExitButton);
    connect(ui->SignUpButton, &QPushButton::clicked, this, &Connection::clickSignUpButton);



    exit = false;
    exec();
}

Connection::~Connection()
{
    delete ui;
}

void Connection::clickSendButton() {
    close();
}

void Connection::clickExitButton()  {
    exit = true;
    close();
}

void Connection::clickSignUpButton() {

}
