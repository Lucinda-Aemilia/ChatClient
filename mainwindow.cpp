#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString username, QHostAddress address,
                       int port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_username(username),
    m_address(address),
    m_port(port)
{
    ui->setupUi(this);
    thread = new QThread;
    worker = new ChatWorker(m_address, m_port, m_username);
    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(start()));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(closed()), worker, SLOT(socketDisconnectSlot()));
    connect(worker, SIGNAL(socketDisconnectedSignal()), this, SLOT(socketDisconnectedSlot()));
    m_startToClose = false;
    thread->start();
}

void MainWindow::socketDisconnectedSlot()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // worker->socketDisconnectSlot();
    if (!m_startToClose)
    {
        qDebug() << "ready to close socket";
        m_startToClose = true;
        emit closed();
    }
    else
    {
        qDebug() << "read to close window";
        QMainWindow::closeEvent(event);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
