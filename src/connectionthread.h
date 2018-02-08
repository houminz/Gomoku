#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include "connection.h"

#include <QThread>

class ConnectionThread : public QThread
{
public:
    ConnectionThread();
};

#endif // CONNECTIONTHREAD_H
