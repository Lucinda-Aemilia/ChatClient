#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chatworker.h"

#include <QMainWindow>
#include <QtNetwork>
#include <QThread>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QStackedWidget>
#include <QMap>

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
    // void writeToSocketSlot();
    void readFromSocket(const QString& info);
    void updateTableWidget();
    void updateListWidget();

signals:
    void closed();
    void writeToSocket(const QString& info);

protected:
    void closeEvent(QCloseEvent* event);

private slots:

    void on_sendButton_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    QString m_username;
    QHostAddress m_address;
    int m_port;

    QThread* thread;
    ChatWorker* worker;
    bool m_startToClose;

    QVector<QString> m_usernameList;
    QVector<qintptr> m_descriptorList;
    QMap<qintptr, QVector<QString> > m_chatRecords;
};

#endif // MAINWINDOW_H
