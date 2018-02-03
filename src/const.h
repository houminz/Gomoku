#ifndef CONST_H
#define CONST_H

#include <QColor>

namespace Const
{

const int TIME_LIMIT = 30;
const int SIZE = 14;

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
}
#endif // CONST_H
