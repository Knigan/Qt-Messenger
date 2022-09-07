#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new TCPServer;

    server->sendData("CREATE TABLE IF NOT EXISTS users (id INTEGER NOT NULL PRIMARY KEY, login TEXT, password TEXT, name TEXT, surname TEXT, status TEXT);");
    //server->sendData("CREATE INDEX IF NOT EXISTS users_index ON users USING BTREE(id);");

    server->sendData("CREATE TABLE IF NOT EXISTS contacts (id INTEGER NOT NULL PRIMARY KEY, user_id INTEGER NOT NULL, contact_id INTEGER NOT NULL);");
    //server->sendData("CREATE INDEX IF NOT EXISTS contacts_index ON contacts USING BTREE(user_id);");

    Connection d(this, server);
    d.exec();
    id = d.getId();

    refreshProfile();
    refreshContacts();

    connect(ui->ProfileApplyButton, &QPushButton::clicked, this, &MainWindow::clickProfileApplyButton);
    connect(ui->ProfileCancelButton, &QPushButton::clicked, this, &MainWindow::clickProfileCancelButton);
    connect(ui->ProfileChangePasswordButton, &QPushButton::clicked, this, &MainWindow::clickProfileChangePasswordButton);

    connect(ui->AddContactButton, &QPushButton::clicked, this, &MainWindow::clickAddContactButton);
    connect(ui->DeleteContactButton, &QPushButton::clicked, this, &MainWindow::clickDeleteContactButton);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshProfile() {
    QString data = server->sendData("SELECT login, name, surname, status FROM users WHERE id = " + QString::number(id) + ";");

    QString profile_login = TCPServer::correct(data.section(',', 0, 0));
    QString profile_name = TCPServer::correct(data.section(',', 1, 1));
    QString profile_surname = TCPServer::correct(data.section(',', 2, 2));
    QString profile_status = TCPServer::correct(data.section(',', 3, 3));

    ui->ProfileLoginLineEdit->setText(profile_login);
    ui->ProfileFirstNameLineEdit->setText(profile_name);
    ui->ProfileLastNameLineEdit->setText(profile_surname);
    ui->ProfileStatusTextEdit->setText(profile_status);
}

void MainWindow::refreshContacts() {
    ui->ContactsTableWidget->clear();

    int count = TCPServer::correct(server->sendData("SELECT COUNT(id) FROM contacts WHERE user_id = " + QString::number(id) + ";")).toInt();
    ui->ContactsTableWidget->setRowCount(count + 1);
    ui->ContactsTableWidget->setColumnCount(3);

    QString data = server->sendData("SELECT users.login, users.surname, users.name FROM users JOIN contacts ON users.id = contacts.contact_id WHERE contacts.user_id = " + QString::number(id) + ";");
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
                         + "' WHERE id = " + QString::number(id) + ";");
        refreshProfile();
        ui->ProfileSuccessLabel->setText("Applied!");
    }
}

void MainWindow::clickProfileCancelButton() {
    refreshProfile();
    ui->ProfileSuccessLabel->setText("Cancelled!");
}

void MainWindow::clickProfileChangePasswordButton() {
    ChangePassword pass(id, this, server);
    pass.exec();
    if (!pass.cancel) {
        refreshProfile();
        ui->ProfileSuccessLabel->setText("The password changed successfully!");
    }
}

void MainWindow::clickAddContactButton() {
    AddContact cnt(id, this, server);
    cnt.exec();
    if (!cnt.cancel) {
        refreshContacts();
        ui->ContactsSuccessLabel->setText("A new contact was added!");
    }
}

void MainWindow::clickDeleteContactButton() {
    DeleteContact cnt(id, this, server);
    cnt.exec();
    if (!cnt.cancel) {
        refreshContacts();
        ui->ContactsSuccessLabel->setText("A contact was deleted!");
    }
}
