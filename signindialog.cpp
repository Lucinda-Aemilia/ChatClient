#include "signindialog.h"
#include "ui_signindialog.h"

SignInDialog::SignInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignInDialog)
{
    ui->setupUi(this);
}

void SignInDialog::accept()
{
    if (ui->usernameLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "The username is empty");
        return;
    }
    if (ui->ipLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "The IP is empty");
        return;
    }
    username = ui->usernameLineEdit->text();
    address = QHostAddress(ui->ipLineEdit->text());
    port = ui->portSpinBox->value();
    QDialog::accept();
}

SignInDialog::~SignInDialog()
{
    delete ui;
}
