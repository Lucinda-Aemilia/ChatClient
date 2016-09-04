#include "chatworker.h"

ChatWorker::ChatWorker(const QHostAddress &hostName, quint16 port,
                       QString username, QObject *parent) :
    m_address(hostName),
    m_username(username),
    m_port(port),
    m_quit(false),
    QObject(parent)
{
}

//! [0]
ChatWorker::~ChatWorker()
{
    mutex.lock();
    m_quit = true;
    mutex.unlock();
    // wait();
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
    const int Timeout = 5 * 1000;

    m_tcpSocket->connectToHost(hostName, port);

    if (!m_tcpSocket->waitForConnected(Timeout))
    {
        emit error(m_tcpSocket->error(), m_tcpSocket->errorString());
        return;
    }

    // 写登录信息
    QDataStream out(m_tcpSocket);
    out.setVersion(QDataStream::Qt_4_0);
    out << username;
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
