#include "configurechat.h"
#include "ui_configurechat.h"

ConfigureChat::ConfigureChat(int ID, QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::ConfigureChat),
    id(ID)
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

    int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM contacts WHERE user_id = " + QString::number(id) + ";")).toInt();
    QString data = server->sendData("SELECT users.login FROM contacts JOIN users ON contacts.contact_id = users.id WHERE contacts.user_id = " + QString::number(id) + ";");
    for (int i = 0; i < count; ++i) {
        ui->ContactComboBox->addItem(TCPServer::correct(data.section('\n', i, i)));
    }

    cancel = false;
}

ConfigureChat::~ConfigureChat()
{
    delete ui;
}

void ConfigureChat::refreshChatsList() {
    ui->ChatComboBox->clear();

    int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM chats WHERE user_id = " + QString::number(id) + ";")).toInt();
    QString data = server->sendData("SELECT name FROM chats WHERE user_id = " + QString::number(id) + ";");
    for (int i = 0; i < count; ++i) {
        ui->ChatComboBox->addItem(TCPServer::correct(data.section('\n', i, i)));
    }
}

void ConfigureChat::clickSendInviteButton() {
    int chat_id = TCPServer::correct(ui->ChatComboBox->currentText().section(':', 1, 1)).toInt();
    int user_id = TCPServer::correct(server->sendData("SELECT id FROM users WHERE login = '" + ui->ContactComboBox->currentText() + "';")).toInt();
    QString check = TCPServer::correct(server->sendData("SELECT id FROM chats WHERE chat_id = " + QString::number(chat_id) + " AND user_id = " + QString::number(user_id) + ";"));
    if (check != "The request was completed successfully") {
        ui->SuccessLabel->setText("This user is already in this chat");
    }
    else {
        int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM chats;")).toInt();
        server->sendData("INSERT INTO chats VALUES (" + QString::number(count + 1) + ", " + QString::number(chat_id) + ", '" + ui->ChatComboBox->currentText()
                         + " : " + QString::number(chat_id) + "', " + QString::number(user_id) + ");");
        ui->SuccessLabel->setText("Invite was sent successfully!");
    }
}

void ConfigureChat::clickRenameChatButton() {
    int chat_id = TCPServer::correct(ui->ChatComboBox->currentText().section(':', 1, 1)).toInt();
    server->sendData("UPDATE chats SET name = '" + ui->RenameLineEdit->text() + " : " + QString::number(chat_id) + "' WHERE chat_id = " + QString::number(chat_id) + ";");
    refreshChatsList();
    ui->SuccessLabel->setText("The chat was renamed successfully!");
}

void ConfigureChat::clickDeleteChatButton() {
    int chat_id = TCPServer::correct(ui->ChatComboBox->currentText().section(':', 1, 1)).toInt();
    server->sendData("DELETE FROM chats WHERE chat_id = " + QString::number(chat_id) + " AND user_id = " + QString::number(id) + ";");
    refreshChatsList();
    ui->SuccessLabel->setText("The chat was deleted successfully!");
}

void ConfigureChat::clickCancelButton() {
    cancel = true;
    close();
}
