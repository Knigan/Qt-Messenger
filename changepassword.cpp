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
    QString password = server->sendData("SELECT password FROM users WHERE id = " + QString::number(id));
    auto correct = [](QString& str) {
        str.remove(QChar('('));
        str.remove(QChar(')'));
        str.replace(QString("'"), QString(""));
        str.remove(QChar('"'));
        str.remove(QChar(','));

        while (str[0] == ' ') {
            str.remove(0, 1);
        }
        while (str[str.length() - 1] == ' ') {
            str.remove(str.length() - 1, 1);
        }
    };
    correct(password);
    if (ui->OldPasswordLineEdit->text() != password) {
        ui->ErrorLabel->setText("You entered the wrong old password!");
    }
    else {
        if (ui->NewPasswordLineEdit->text() == password) {
            ui->ErrorLabel->setText("The new password must differ from the old password!");
        }
        else {
            server->sendData("UPDATE users SET password = '" + ui->NewPasswordLineEdit->text()
                             + "' WHERE id = " + QString::number(id));
            close();
        }
    }
}

void ChangePassword::clickCancelButton() {
    cancel = true;
    close();
}
