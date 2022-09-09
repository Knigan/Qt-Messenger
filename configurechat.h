#ifndef CONFIGURECHAT_H
#define CONFIGURECHAT_H

#include <QDialog>
#include "tcpserver.h"

namespace Ui {
class ConfigureChat;
}

class ConfigureChat : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureChat(int ID, QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~ConfigureChat();
    void refreshChatsList();
    bool cancel;

private slots:
    void clickSendInviteButton();
    void clickRenameChatButton();
    void clickDeleteChatButton();
    void clickCancelButton();

private:
    Ui::ConfigureChat *ui;
    TCPServer* server;
    int id;
};

#endif // CONFIGURECHAT_H
