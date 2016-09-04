#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chatworker.h"

#include <QMainWindow>
#include <QtNetwork>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString username, QHostAddress address,
                        int port, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void socketDisconnectedSlot();

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;
    QString m_username;
    QHostAddress m_address;
    int m_port;

    QThread* thread;
    ChatWorker* worker;
    bool m_startToClose;
};

#endif // MAINWINDOW_H
