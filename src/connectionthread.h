#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include "connection.h"
#include "const.h"

#include <QThread>

class ConnectionThread : public QThread
{
    Q_OBJECT

public:
    explicit ConnectionThread(Const::HostType type, QObject* parent = nullptr);
    explicit ConnectionThread(Const::HostType type, const QString& ip, int port, QObject *parent = nullptr);
    ~ConnectionThread();

    Connection* getConnection() const { return m_connection; }
    void setSocketDescriptor(qintptr socketDescriptor) { m_socketDescriptor = socketDescriptor; }
    void setGreetingMessage(const QString& message) { m_greeting_message = message; }

protected:
    void run() override;

private:
    Const::HostType m_type;
    QString m_ip, m_greeting_message;
    int m_port;
    qintptr m_socketDescriptor;
    Connection* m_connection;

signals:
    void connectionReady(const QString& oppUsername);
};

#endif // CONNECTIONTHREAD_H
