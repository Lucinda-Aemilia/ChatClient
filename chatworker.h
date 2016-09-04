#ifndef CHATWORKER_H
#define CHATWORKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

//! [0]
class ChatWorker : public QObject
{
    Q_OBJECT

public:
    ChatWorker(const QHostAddress &hostName, quint16 port, QString username, QObject *parent = 0);
    ~ChatWorker();

public slots:
    void start();
    void run();

signals:
    void newFortune(const QString &fortune);
    void error(int socketError, const QString &message);

private:
    QHostAddress m_address;
    QString m_username;
    QMutex mutex;
    QTcpSocket* m_tcpSocket;
    quint16 m_port;
    bool m_quit;
};
//! [0]

#endif // CHATWORKER_H
