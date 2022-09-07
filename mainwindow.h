#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"
#include "connection.h"
#include "changepassword.h"
#include "addcontact.h"
#include "deletecontact.h"

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
    void refreshContacts();

private slots:
    void clickProfileApplyButton();
    void clickProfileCancelButton();
    void clickProfileChangePasswordButton();
    void clickAddContactButton();
    void clickDeleteContactButton();
    void on_actionReconnect_triggered();
    void on_actionRefresh_triggered();
    void clickedContact(const QModelIndex&);

private:
    Ui::MainWindow *ui;
    TCPServer* server;
    int id;
};
#endif // MAINWINDOW_H
