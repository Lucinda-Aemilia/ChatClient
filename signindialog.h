#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QHostAddress>

namespace Ui {
class SignInDialog;
}

class SignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignInDialog(QWidget *parent = 0);
    ~SignInDialog();
    QString getUsername() const { return username; }
    QHostAddress getAddress() const { return address; }
    int getPort() const { return port; }

public slots:
    void accept();

private:
    Ui::SignInDialog *ui;
    QString username;
    QHostAddress address;
    int port;
};

#endif // SIGNINDIALOG_H
