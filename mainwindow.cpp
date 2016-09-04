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
    QThread* thread = new QThread;
    ChatWorker* worker = new ChatWorker(m_address, m_port, m_username);
    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(start()));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
