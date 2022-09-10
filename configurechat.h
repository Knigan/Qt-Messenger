#ifndef CONFIGURECHAT_H
#define CONFIGURECHAT_H

#include <QDialog>
#include "tcpserver.h"
#include <QDateTime>

namespace Ui {
class ConfigureChat;
}

class ConfigureChat : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureChat(int ID, QString Surname, QString Name, QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~ConfigureChat();
    void refreshChatsList();

private slots:
    void clickSendInviteButton();
    void clickRenameChatButton();
    void clickDeleteChatButton();
    void clickCancelButton();

private:
    Ui::ConfigureChat *ui;
    TCPServer* server;

    int id;
    QString surname;
    QString name;
};

#endif // CONFIGURECHAT_H
