#include "createchat.h"
#include "ui_createchat.h"

CreateChat::CreateChat(int ID, QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::CreateChat),
    id(ID)
{
    ui->setupUi(this);

    if (s == nullptr) {
        server = new TCPServer;
    }
    else {
        server = s;
    }

    connect(ui->CreateChatButton, &QPushButton::clicked, this, &CreateChat::clickCreateChatButton);
    connect(ui->CancelButton, &QPushButton::clicked, this, &CreateChat::clickCancelButton);

    cancel = false;
}

CreateChat::~CreateChat()
{
    delete ui;
}

void CreateChat::clickCreateChatButton() {
    QString name = ui->ChatNameLineEdit->text();
    if (name == "") {
        ui->ErrorLabel->setText("Chat name must contain at least 1 character");
    }
    else {
        if (name.contains(':')) {
            ui->ErrorLabel->setText("Chat name cannot contain a ':' character");
        }
        else {
            int count = TCPServer::correct(server->sendData("SELECT MAX(id) FROM chats;")).toInt();
            QString str = TCPServer::correct(server->sendData("SELECT MAX(chat_id) FROM chats;"));
            int chat_count;
            if (str == "None") {
                chat_count = 0;
            }
            else {
                chat_count = str.toInt();
            }
            server->sendData("INSERT INTO chats VALUES (" + QString::number(count + 1) + ", " + QString::number(chat_count + 1) + ", '" + name + " : " + QString::number(chat_count + 1) + "', " + QString::number(id) + ");");
            close();
        }
    }
}

void CreateChat::clickCancelButton() {
    cancel = true;
    close();
}
