#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    server = new TCPServer;
    Connection d(this, server);
    d.exec();
    id = d.getId();

    refreshProfile();

    connect(ui->ProfileApplyButton, &QPushButton::clicked, this, &MainWindow::clickProfileApplyButton);
    connect(ui->ProfileCancelButton, &QPushButton::clicked, this, &MainWindow::clickProfileCancelButton);
    connect(ui->ProfileChangePasswordButton, &QPushButton::clicked, this, &MainWindow::clickProfileChangePasswordButton);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::correct(QString& str) {
    str.remove(QChar('('));
    str.remove(QChar(')'));
    str.replace(QString("'"), QString(""));
    str.remove(QChar('"'));

    while (str[0] == ' ') {
        str.remove(0, 1);
    }
    while (str[str.length() - 1] == ' ') {
        str.remove(str.length() - 1, 1);
    }
}

void MainWindow::refreshProfile() {
    QString data = server->sendData("SELECT login, name, surname, status FROM users WHERE id = " + QString::number(id) + ";");
    correct(data);

    QString profile_login = data.section(',', 0, 0);
    QString profile_name = data.section(',', 1, 1);
    QString profile_surname = data.section(',', 2, 2);
    QString profile_status = data.section(',', 3, 3);

    correct(profile_name);
    correct(profile_surname);
    correct(profile_status);

    ui->ProfileLoginLineEdit->setText(profile_login);
    ui->ProfileFirstNameLineEdit->setText(profile_name);
    ui->ProfileLastNameLineEdit->setText(profile_surname);
    ui->ProfileStatusTextEdit->setText(profile_status);
}

void MainWindow::clickProfileApplyButton() {
    server->sendData("UPDATE users SET login = '" + ui->ProfileLoginLineEdit->text()
                     + "', name = '" + ui->ProfileFirstNameLineEdit->text()
                     + "', surname = '" + ui->ProfileLastNameLineEdit->text()
                     + "', status = '" + ui->ProfileStatusTextEdit->toPlainText()
                     + "' WHERE id = " + QString::number(id));
    refreshProfile();
    ui->SuccessLabel->setText("Applied!");
}

void MainWindow::clickProfileCancelButton() {
    refreshProfile();
    ui->SuccessLabel->setText("Cancelled!");
}

void MainWindow::clickProfileChangePasswordButton() {
    ChangePassword cp(id, this, server);
    cp.exec();
    if (!cp.cancel) {
        refreshProfile();
        ui->SuccessLabel->setText("The password changed successfully!");
    }
}
