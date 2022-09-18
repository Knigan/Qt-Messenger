#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include "tcpserver.h"
#include "connection.h"
#include "changepassword.h"
#include "addcontact.h"
#include "deletecontact.h"
#include "createchat.h"
#include "configurechat.h"

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
    void refreshChatsList();
    void refreshChat(int chat_id, bool flag = true);

private slots:
    void clickProfileApplyButton();
    void clickProfileCancelButton();
    void clickProfileChangePasswordButton();

    void clickAddContactButton();
    void clickDeleteContactButton();
    void clickedContact(const QModelIndex&);

    void on_actionReconnect_triggered();
    void on_actionRefresh_triggered();  

    void clickChatsSendButton();
    void clickChatsCreateChatButton();
    void clickChatsConfigureChatButton();
    void clickedChat(const QModelIndex&);
    void clickRefreshChatButton();

private:
    Ui::MainWindow *ui;
    TCPServer* server;
    int chat_id;
    QTimer* timer;

    struct User {
        int id;
        QString login;
        QString name;
        QString surname;
        QString status;
    };

    User u;
};
#endif // MAINWINDOW_H
