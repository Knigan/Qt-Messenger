#include "changepassword.h"
#include "ui_changepassword.h"

ChangePassword::ChangePassword(int ID, QWidget *parent, TCPServer* s) :
    QDialog(parent),
    ui(new Ui::ChangePassword),
    id(ID)
{
    ui->setupUi(this);

    if (s == nullptr) {
        server = new TCPServer;
    }
    else {
        server = s;
    }

    connect(ui->ApplyButton, &QPushButton::clicked, this, &ChangePassword::clickApplyButton);
    connect(ui->CancelButton, &QPushButton::clicked, this, &ChangePassword::clickCancelButton);

    cancel = false;
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::clickApplyButton() {
    bool flag = true;
    QString old_password = TCPServer::correct(server->sendData("SELECT password FROM users WHERE id = " + QString::number(id)));
    QString new_password = ui->NewPasswordLineEdit->text();
    if (ui->OldPasswordLineEdit->text() != old_password) {
        ui->ErrorLabel->setText("You entered the wrong old password!");
    }
    else {
        if (new_password != TCPServer::correct(new_password)) {
            ui->ErrorLabel->setText("Password contains restricted characters or excess space symbols");
            flag = false;
        }
        if (flag) {
            if (new_password == old_password) {
                ui->ErrorLabel->setText("The new password must differ from the old password!");
            }
            else {
                server->sendData("UPDATE users SET password = '" + ui->NewPasswordLineEdit->text()
                                 + "' WHERE id = " + QString::number(id));
                close();
            }
        }
    }
}

void ChangePassword::clickCancelButton() {
    cancel = true;
    close();
}
