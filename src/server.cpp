#include "server.h"

Server::Server(QString ip, int port, QObject *parent) :
    QTcpServer(parent)
{
    listen(QHostAddress(ip), port);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    ConnectionThread* thread = new ConnectionThread(Const::Server, this);
    thread->setSocketDescriptor(socketDescriptor);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    emit newConnection(thread);
}
