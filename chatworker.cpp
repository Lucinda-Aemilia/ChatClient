#include "chatworker.h"

ChatWorker::ChatWorker(const QHostAddress &hostName, quint16 port,
                       QString username, QObject *parent) :
    QObject(parent),
    m_address(hostName),
    m_username(username),
    m_port(port),
    m_quit(false)

{
    m_tcpSocket = NULL;
}

//! [0]
ChatWorker::~ChatWorker()
{
    mutex.lock();
    m_quit = true;
    mutex.unlock();
    // wait();
    if (m_tcpSocket)
    {
        qDebug() << "wait for disconnected";
        m_tcpSocket->waitForDisconnected();
    }
}
//! [0]

void ChatWorker::start()
{
    QMutexLocker locker(&mutex);
    QHostAddress hostName(this->m_address);
    int port = this->m_port;
    QString username(this->m_username);
    locker.unlock();

    this->m_tcpSocket = new QTcpSocket;
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readAll()));
    const int Timeout = 5 * 1000;

    m_tcpSocket->connectToHost(hostName, port);

    if (!m_tcpSocket->waitForConnected(Timeout))
    {
        emit error(m_tcpSocket->error(), m_tcpSocket->errorString());
        return;
    }

    qDebug() << "ChatWorker::start()" << "connected" << m_tcpSocket->isOpen();

    // 写登录信息
    QString str(QString("USERNAME`%1").arg(username));
    m_tcpSocket->write(str.toLocal8Bit());
}

void ChatWorker::socketDisconnectSlot()
{
    qDebug() << "wait for disconnected" << m_tcpSocket;
    m_tcpSocket->write(QString("QUIT").toLocal8Bit());
    qDebug() << "written quit";
    if (m_tcpSocket != NULL)
    {
        // qDebug() << "???";
        m_tcpSocket->disconnectFromHost();
        qDebug() << m_tcpSocket->state();
        bool disconnected;
        if (m_tcpSocket->state() == QAbstractSocket::UnconnectedState)
            disconnected = true;
        while (m_tcpSocket->state() != QAbstractSocket::UnconnectedState)
        {
            disconnected = m_tcpSocket->waitForDisconnected(1000);
            if (disconnected)
                break;
        }
        // bool disconnected = m_tcpSocket->waitForDisconnected();
        qDebug() << "have been disconnected" << disconnected;
        emit socketDisconnectedSignal();
    }
}

void ChatWorker::readAll()
{
    QString info(m_tcpSocket->readAll());
    qDebug() << "ChatWorker::readAll()" << info;
    emit readFromSocket(info);
}

//! [4]
void ChatWorker::run()
{
    mutex.lock();
    // QHostAddress address(m_address);
    // quint16 serverPort = m_port;
    mutex.unlock();

    while (!m_quit)
    {

        while (m_tcpSocket->bytesAvailable() < (int)sizeof(quint16))
        {
            if (!m_tcpSocket->waitForReadyRead(5000))
            {
                emit error(m_tcpSocket->error(), m_tcpSocket->errorString());
                return;
            }
        }

        quint16 blockSize;
        QDataStream in(m_tcpSocket);
        in.setVersion(QDataStream::Qt_4_0);
        in >> blockSize;

        while (m_tcpSocket->bytesAvailable() < blockSize)
        {
            if (!m_tcpSocket->waitForReadyRead(5000)) {
                emit error(m_tcpSocket->error(), m_tcpSocket->errorString());
                return;
            }
        }
    }
}
