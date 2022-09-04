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

void MainWindow::refreshProfile() {
    QString profile_login = server->sendData("SELECT login FROM users WHERE id = " + QString::number(id) + ";");
    QString profile_name = server->sendData("SELECT name FROM users WHERE id = " + QString::number(id) + ";");
    QString profile_surname = server->sendData("SELECT surname FROM users WHERE id = " + QString::number(id) + ";");
    QString profile_status = server->sendData("SELECT status FROM users WHERE id = " + QString::number(id) + ";");

    ui->ProfileLoginLineEdit->setText(profile_login);
    ui->ProfileFirstNameLineEdit->setText(profile_name);
    ui->ProfileLastNameLineEdit->setText(profile_surname);
    ui->ProfileStatusTextEdit->setText(profile_status);
}

void MainWindow::clickProfileApplyButton() {
    server->sendData("UPDATE users SET login = " + ui->ProfileLoginLineEdit->text()
                     + ", name = " + ui->ProfileFirstNameLineEdit->text()
                     + ", surname = " + ui->ProfileLastNameLineEdit->text()
                     + ", status = " + ui->ProfileStatusTextEdit->toPlainText()
                     + " WHERE id = " + QString::number(id));
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
    }
    ui->SuccessLabel->setText("The password changed successfully!");
}
