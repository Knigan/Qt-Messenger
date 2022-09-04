#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"
#include "connection.h"
#include "changepassword.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshProfile();

private slots:
    void clickProfileApplyButton();
    void clickProfileCancelButton();
    void clickProfileChangePasswordButton();

private:
    Ui::MainWindow *ui;
    TCPServer* server;
    int id;
};
#endif // MAINWINDOW_H
