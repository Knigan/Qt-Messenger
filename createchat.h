#ifndef CREATECHAT_H
#define CREATECHAT_H

#include <QDialog>
#include "tcpserver.h"

namespace Ui {
class CreateChat;
}

class CreateChat : public QDialog
{
    Q_OBJECT

public:
    explicit CreateChat(int ID, QWidget *parent = nullptr, TCPServer* s = nullptr);
    ~CreateChat();

    bool cancel;

private slots:
    void clickCreateChatButton();
    void clickCancelButton();

private:
    Ui::CreateChat *ui;
    TCPServer* server;
    int id;
};

#endif // CREATECHAT_H
