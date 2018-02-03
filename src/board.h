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

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;

private:
    void placePiece(int row, int col, Piece::PieceColor color);
    bool checkWin(int row, int col, Piece::PieceColor color);

    Piece m_board[Const::SIZE + 1][Const::SIZE + 1];
    QPointF m_center;
    Piece::PieceColor m_color;
    bool m_is_hidden;
    bool m_is_block;
    int m_round;
    double m_cell_width;

    bool m_bomb[Const::SIZE + 1][Const::SIZE + 1];
};

#endif
