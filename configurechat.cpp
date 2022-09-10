#include "configurechat.h"
#include "ui_configurechat.h"

ConfigureChat::ConfigureChat(int ID, QString Surname, QString Name, QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::ConfigureChat),
    id(ID),
    surname(Surname),
    name(Name)
{
    ui->setupUi(this);

    if (s == nullptr) {
        server = new TCPServer;
    }
    else {
        server = s;
    }

    connect(ui->SendInviteButton, &QPushButton::clicked, this, &ConfigureChat::clickSendInviteButton);
    connect(ui->RenameChatButton, &QPushButton::clicked, this, &ConfigureChat::clickRenameChatButton);
    connect(ui->DeleteChatButton, &QPushButton::clicked, this, &ConfigureChat::clickDeleteChatButton);
    connect(ui->CancelButton, &QPushButton::clicked, this, &ConfigureChat::clickCancelButton);

    refreshChatsList();

    ui->ContactComboBox->clear();

    QString data = server->sendData("SELECT users.login FROM contacts JOIN users ON contacts.contact_id = users.id WHERE contacts.user_id = " + QString::number(id) + ";");
    for (int i = 0; i < data.count("[?~?]"); ++i) {
        ui->ContactComboBox->addItem(TCPServer::correct(data.section("[?~?]", i, i)));
    }
}

ConfigureChat::~ConfigureChat()
{
    delete ui;
}

void ConfigureChat::refreshChatsList() {
    ui->ChatComboBox->clear();

    QString data = server->sendData("SELECT name FROM chats WHERE user_id = " + QString::number(id) + ";");
    for (int i = 0; i < data.count("[?~?]"); ++i) {
        ui->ChatComboBox->addItem(TCPServer::correct(data.section("[?~?]", i, i)));
    }
}

void ConfigureChat::clickSendInviteButton() {
    QString name = ui->ChatComboBox->currentText();
    int chat_id = TCPServer::correct(name.section(':', 1, 1)).toInt();
    int user_id = TCPServer::correct(server->sendData("SELECT id FROM users WHERE login = '" + ui->ContactComboBox->currentText() + "';")).toInt();
    QString check = TCPServer::correct(server->sendData("SELECT id FROM chats WHERE chat_id = " + QString::number(chat_id) + " AND user_id = " + QString::number(user_id) + ";"));
    if (check != "The request was completed successfully") {
        ui->SuccessLabel->setText("This user is already in this chat");
    }
    else {
        int count = TCPServer::correct(server->sendData("SELECT COUNT(*) FROM chats;")).toInt();
        server->sendData("INSERT INTO chats VALUES (" + QString::number(count + 1) + ", " + QString::number(chat_id) + ", '" + name
                         + "', " + QString::number(user_id) + ");");
        ui->SuccessLabel->setText("Invite was sent successfully!");
    }
}

void ConfigureChat::clickRenameChatButton() {
    int chat_id = TCPServer::correct(ui->ChatComboBox->currentText().section(':', 1, 1)).toInt();
    QString old_name = TCPServer::correct(server->sendData("SELECT name FROM chats WHERE chat_id = " + QString::number(chat_id) + ";")).section(':', 0, 0);
    QString new_name = ui->RenameLineEdit->text();
    if (new_name.contains(':')) {
        ui->SuccessLabel->setText("Chat name cannot contain a ':' character");
    }
    else {
        server->sendData("UPDATE chats SET name = '" + new_name + " : " + QString::number(chat_id) + "' WHERE chat_id = " + QString::number(chat_id) + ";");

        int count = TCPServer::correct(server->sendData("SELECT COUNT(*) FROM chatcontent;")).toInt();

        server->sendData("INSERT INTO chatcontent VALUES (" + QString::number(count + 1) + ", " + QString::number(chat_id) + ", 'System " + "(" +
                         QDateTime::currentDateTime().toString("dd.MM.yy HH:mm:ss") + "):[?~?]" + surname + " " + name + " renamed this chat (" + old_name + " -> " + new_name + ")');");

        refreshChatsList();
        ui->SuccessLabel->setText("The chat was renamed successfully!");
    }
}

void ConfigureChat::clickDeleteChatButton() {
    int chat_id = TCPServer::correct(ui->ChatComboBox->currentText().section(':', 1, 1)).toInt();
    server->sendData("DELETE FROM chats WHERE chat_id = " + QString::number(chat_id) + " AND user_id = " + QString::number(id) + ";");

    int check_count = TCPServer::correct(server->sendData("SELECT COUNT(*) FROM chats WHERE chat_id = " + QString::number(chat_id) + ";")).toInt();
    if (check_count == 0) {
        server->sendData("DELETE FROM chatcontent WHERE chat_id = " + QString::number(chat_id) + ";");
    }
    else {
        int count = TCPServer::correct(server->sendData("SELECT COUNT(*) FROM chatcontent;")).toInt();

        server->sendData("INSERT INTO chatcontent VALUES (" + QString::number(count + 1) + ", " + QString::number(chat_id) + ", 'System " + "(" +
                         QDateTime::currentDateTime().toString("dd.MM.yy HH:mm:ss") + "):[?~?]" + surname + " " + name + " left from this chat');");
    }

    refreshChatsList();
    ui->SuccessLabel->setText("The chat was deleted successfully!");
}

void ConfigureChat::clickCancelButton() {
    close();
}
