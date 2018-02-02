#ifndef BOARD_H
#define BOARD_H

#include "const.h"
#include "piece.h"

#include <QWidget>

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    void clearBoard();
protected:
    void paintEvent(QPaintEvent *event);

private:
    bool isOnBoard(int x, int y);
    bool isAvailable(int x, int y);
    bool checkWin(int row, int col, int color);
    bool hasBomb(int x, int y);

    Piece m_board[Const::SIZE][Const::SIZE];
    bool m_is_hidden;
    int m_round;
    double m_cell_width;

};

#endif
