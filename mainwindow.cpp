#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket;
    Connection d(this, socket);
    if (d.exit) {
        close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

