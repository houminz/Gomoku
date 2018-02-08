#ifndef CONST_H
#define CONST_H

#include <QColor>
#include <QHostInfo>
#include <QNetworkInterface>

namespace Const
{

const int TIME_LIMIT = 30;
const int SIZE = 14;

enum HostType
{
    Server,
    Client
};

const QColor PIECE_COLOR[3] =
{
    Qt::transparent,
    Qt::white,
    Qt::black
};

const int POINT[5][2] =
{
    {0,  0 },
    {4,  4 },
    {4,  -4},
    {-4, 4 },
    {-4, -4}
};

inline double Sqr(double x) { return x * x; }

inline QString getLocalIP()
{
#ifdef Q_OS_WIN
    auto list = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
#else
    auto list = QNetworkInterface::allAddresses();
#endif
    for (auto i : list)
        if (i != QHostAddress::LocalHost && i.protocol() == QAbstractSocket::IPv4Protocol)
            return i.toString();
    return "";
}
}
#endif // CONST_H
