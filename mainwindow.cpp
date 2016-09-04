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
    connect(worker, SIGNAL(readFromSocket(QString)), this, SLOT(readFromSocket(QString)));
    m_startToClose = false;
    thread->start();
}

void MainWindow::readFromSocket(const QString &info)
{
    qDebug() << "MainWindow::readFromSocket(const QString &info)" << info;
    QStringList list(info.split('`'));
    if (list.at(0) == "LIST")
    {
        m_descriptorList.clear();
        m_usernameList.clear();
        for (int i = 1; i < list.size() - 1; i += 2)
        {
            m_descriptorList.append(list.at(i).toInt());
            m_usernameList.append(list.at(i+1));
        }
        updateTableWidget();
    }
}

void MainWindow::updateTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(2);
    QStringList labels;
    labels.append("Socket Descriptor");
    labels.append("Username");
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->setRowCount(m_usernameList.size());
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    for (int i = 0; i < m_descriptorList.size(); i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(
                    QString::number(m_descriptorList.at(i)));
        ui->tableWidget->setItem(i, 0, newItem);

        newItem = new QTableWidgetItem(m_usernameList.at(i));
        ui->tableWidget->setItem(i, 1, newItem);
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
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
