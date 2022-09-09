#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new TCPServer;

    server->sendData("CREATE TABLE IF NOT EXISTS users (id INTEGER NOT NULL PRIMARY KEY, login TEXT, password TEXT, name TEXT, surname TEXT, status TEXT);");
    server->sendData("CREATE TABLE IF NOT EXISTS contacts (id INTEGER NOT NULL PRIMARY KEY, user_id INTEGER NOT NULL, contact_id INTEGER NOT NULL);");
    server->sendData("CREATE TABLE IF NOT EXISTS chats (id INTEGER NOT NULL PRIMARY KEY, chat_id INTEGER NOT NULL, name text, user_id INTEGER NOT NULL);");
    server->sendData("CREATE TABLE IF NOT EXISTS chatcontent (id INTEGER NOT NULL PRIMARY KEY, chat_id INTEGER NOT NULL, message text);");

    server->sendData("CREATE INDEX IF NOT EXISTS users_index ON users(id);");
    server->sendData("CREATE INDEX IF NOT EXISTS contacts_index ON contacts(user_id);");
    server->sendData("CREATE INDEX IF NOT EXISTS chats_index ON chats(chat_id);");
    server->sendData("CREATE INDEX IF NOT EXISTS chatcontent_index ON chatcontent(chat_id);");

    Connection d(this, server);
    d.exec();
    u.id = d.getId();

    refreshProfile();
    refreshContacts();
    refreshChatsList();

    connect(ui->ProfileApplyButton, &QPushButton::clicked, this, &MainWindow::clickProfileApplyButton);
    connect(ui->ProfileCancelButton, &QPushButton::clicked, this, &MainWindow::clickProfileCancelButton);
    connect(ui->ProfileChangePasswordButton, &QPushButton::clicked, this, &MainWindow::clickProfileChangePasswordButton);

    connect(ui->AddContactButton, &QPushButton::clicked, this, &MainWindow::clickAddContactButton);
    connect(ui->DeleteContactButton, &QPushButton::clicked, this, &MainWindow::clickDeleteContactButton); 
    connect(ui->ContactsTableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedContact(const QModelIndex&)));

    connect(ui->ChatsBackButton, &QPushButton::clicked, this, &MainWindow::refreshChatsList);
    connect(ui->ChatsSendButton, &QPushButton::clicked, this, &MainWindow::clickChatsSendButton);
    connect(ui->ChatsCreateChatButton, &QPushButton::clicked, this, &MainWindow::clickChatsCreateChatButton);
    connect(ui->ChatsConfigureChatButton, &QPushButton::clicked, this, &MainWindow::clickChatsConfigureChatButton);
    connect(ui->ChatsListWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedChat(const QModelIndex&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshProfile() {
    QString data = server->sendData("SELECT login, name, surname, status FROM users WHERE id = " + QString::number(u.id) + ";");

    QString profile_login = TCPServer::correct(data.section(',', 0, 0));
    QString profile_name = TCPServer::correct(data.section(',', 1, 1));
    QString profile_surname = TCPServer::correct(data.section(',', 2, 2));
    QString profile_status = TCPServer::correct(data.section(',', 3, 3));

    u.login = profile_login;
    u.name = profile_name;
    u.surname = profile_surname;
    u.status = profile_status;

    ui->ProfileLoginLineEdit->setText(profile_login);
    ui->ProfileFirstNameLineEdit->setText(profile_name);
    ui->ProfileLastNameLineEdit->setText(profile_surname);
    ui->ProfileStatusTextEdit->setText(profile_status);
}

void MainWindow::refreshContacts() {
    ui->ContactsTableWidget->clear();

    int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM contacts WHERE user_id = " + QString::number(u.id) + ";")).toInt();
    ui->ContactsTableWidget->setRowCount(count + 1);
    ui->ContactsTableWidget->setColumnCount(3);

    QString data = server->sendData("SELECT users.login, users.surname, users.name FROM users JOIN contacts ON users.id = contacts.contact_id WHERE contacts.user_id = " + QString::number(u.id) + ";");
    QVector<QString> vector;

    for (int i = 0; i < count; ++i) {
        vector.append(data.section('\n', i, i));
    }

    ui->ContactsTableWidget->setItem(0, 0, new QTableWidgetItem("Login"));
    ui->ContactsTableWidget->setItem(0, 1, new QTableWidgetItem("Surname"));
    ui->ContactsTableWidget->setItem(0, 2, new QTableWidgetItem("Name"));

    for (int i = 1; i <= count; ++i) {
        for (int j = 0; j < 3; ++j) {
            ui->ContactsTableWidget->setItem(i, j, new QTableWidgetItem(TCPServer::correct(vector[i - 1].section(',', j, j))));
        }
    }
}

void MainWindow::refreshChatsList() {
    ui->ChatsListWidget->clear();
    chat_id = 0;

    int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM chats WHERE user_id = " + QString::number(u.id) + ";")).toInt();
    QString data = server->sendData("SELECT name FROM chats WHERE user_id = " + QString::number(u.id) + ";");

    for (int i = 0; i < count; ++i) {
        ui->ChatsListWidget->addItem(TCPServer::correct(data.section('\n', i, i)));
    }
}

void MainWindow::refreshChat(int chatID) {
    ui->ChatsListWidget->clear();
    chat_id = chatID;

    int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM chatcontent WHERE chatcontent.chat_id = " + QString::number(chat_id) + ";")).toInt();
    QString data = server->sendData("SELECT chatcontent.message FROM chatcontent JOIN chats ON chatcontent.chat_id = chats.chat_id WHERE chatcontent.chat_id = " + QString::number(chat_id));


    for (int i = 0; i < count; ++i) {
        ui->ChatsListWidget->addItem(TCPServer::correct(data.section('\n', i, i)));
    }
}

void MainWindow::clickProfileApplyButton() {
    QString login = ui->ProfileLoginLineEdit->text();
    if (login.length() == 0) {
        ui->ProfileSuccessLabel->setText("The login must contain at least 1 character");
    }
    else {
        server->sendData("UPDATE users SET login = '" + ui->ProfileLoginLineEdit->text()
                         + "', name = '" + ui->ProfileFirstNameLineEdit->text()
                         + "', surname = '" + ui->ProfileLastNameLineEdit->text()
                         + "', status = '" + ui->ProfileStatusTextEdit->toPlainText()
                         + "' WHERE id = " + QString::number(u.id) + ";");
        refreshProfile();
        ui->ProfileSuccessLabel->setText("Applied!");
    }
}

void MainWindow::clickProfileCancelButton() {
    refreshProfile();
    ui->ProfileSuccessLabel->setText("Cancelled!");
}

void MainWindow::clickProfileChangePasswordButton() {
    ChangePassword pass(u.id, this, server);
    pass.exec();
    if (!pass.cancel) {
        refreshProfile();
        ui->ProfileSuccessLabel->setText("The password changed successfully!");
    }
}

void MainWindow::clickAddContactButton() {
    AddContact cnt(u.id, this, server);
    cnt.exec();
    if (!cnt.cancel) {
        refreshContacts();
        ui->ContactsSuccessLabel->setText("A new contact was added!");
    }
}

void MainWindow::clickDeleteContactButton() {
    DeleteContact cnt(u.id, this, server);
    cnt.exec();
    if (!cnt.cancel) {
        refreshContacts();
        ui->ContactsSuccessLabel->setText("A contact was deleted!");
    }
}

void MainWindow::on_actionReconnect_triggered() {
    Connection d(this, server);
    d.exec();
    u.id = d.getId();

    refreshProfile();
    refreshContacts();
    refreshChatsList();
}


void MainWindow::on_actionRefresh_triggered() {
    refreshProfile();
    refreshContacts();
    refreshChatsList();
}

void MainWindow::clickedContact(const QModelIndex& index) {
    QString status = TCPServer::correct(server->sendData("SELECT status FROM users WHERE login = '" + index.data().toString() + "';"));
    if (status != "The request was completed successfully") {
        ui->ContactsTextEdit->setText(status);
    }
}

void MainWindow::clickChatsSendButton() {
    if (chat_id == 0) {
        return;
    }
    QString data = ui->ChatsTextEdit->toPlainText();
    if (data.length() != 0) {
        int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM chatcontent;")).toInt();
        server->sendData("INSERT INTO chatcontent VALUES (" + QString::number(count) + ", " + QString::number(chat_id) + ", '" + QTime::currentTime().toString("HH:mm") + " " + u.login + ":" + data + "');");
    }
    refreshChat(chat_id);
}

void MainWindow::clickChatsCreateChatButton() {
    CreateChat c(u.id, this, server);
    c.exec();
    if (!c.cancel) {
        refreshChatsList();
        ui->ChatsSuccessLabel->setText("The chat was created successfully!");
    }
}

void MainWindow::clickedChat(const QModelIndex& index) {
    int chatID = TCPServer::correct(index.data().toString().section(':', 1, 1)).toInt();
    refreshChat(chatID);
}

void MainWindow::clickChatsConfigureChatButton() {
    ConfigureChat c(u.id, this, server);
    c.exec();
    refreshChatsList();
}
