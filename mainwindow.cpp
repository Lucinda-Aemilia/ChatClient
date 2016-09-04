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
    connect(this, SIGNAL(writeToSocket(QString)),
            worker, SLOT(writeToSocket(QString)));

    ui->usernameLabel->setText(m_username);
    connect(ui->recordList, SIGNAL(currentRowChanged(int)), this, SLOT(updateListWidget()));
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
            qintptr descriptor = list.at(i).toInt();
            m_descriptorList.append(descriptor);
            m_usernameList.append(list.at(i+1));
            // 聊天记录
            if (!m_chatRecords.contains(descriptor))
                m_chatRecords.insert(descriptor, QVector<QString>());
        }


        updateTableWidget();
    }
    else if (list.at(0) == "INFO_FROM")
    {
        qintptr descriptor = list.at(1).toInt();
        QVector<QString> records(m_chatRecords.value(descriptor));
        records.append(list.at(2));
        m_chatRecords.insert(descriptor, records);

        updateListWidget();
    }
}

void MainWindow::updateListWidget()
{
    int index = ui->tableWidget->currentRow();
    if (index < 0)
        return;
    qintptr descriptor = m_descriptorList.at(index);
    QVector<QString> records(m_chatRecords.value(descriptor));
    ui->recordList->clear();
    for (int i = 0; i < records.size(); i++)
    {
        ui->recordList->addItem(records.at(i));
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
    ui->tableWidget->selectRow(0);
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

void MainWindow::on_sendButton_clicked()
{
    int index = ui->tableWidget->currentRow();
    QString text(ui->sendTextEdit->toPlainText());
    text.remove("`");
    qintptr descriptor = m_descriptorList.at(index);

    QString record(QString("%1: %2").arg(m_username).arg(text));
    QVector<QString> records(m_chatRecords.value(descriptor));
    records.append(record);
    // ui->recordList->addItem(records.last());
    m_chatRecords.insert(descriptor, records);

    QString info(QString("INFO_TO`%1`%2").arg(descriptor).arg(record));
    // this->worker->writeToSocket(info);
    emit writeToSocket(info);

    ui->sendTextEdit->clear();
    updateListWidget();
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    QPoint(row, column);
    updateListWidget();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    QPoint(row, column);
    updateListWidget();
}
