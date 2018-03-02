#ifndef BOARD_H
#define BOARD_H

#include "const.h"
#include "piece.h"

#include <QWidget>
#include <QStack>
#include <QTimer>

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    // getter member functions
    int getMyPieces() const { return m_my_pieces; }
    Piece::PieceColor getColor() { return m_color; }

    // setter member functions
    void setColor(Piece::PieceColor color) { m_color = color; }
    void setBlock(bool block) { m_is_blocked = block; }
    void setHidden(bool hidden) { m_is_hidden = hidden; }

    void clear();
    void placePiece(int row, int col, Piece::PieceColor color);
    void undo(int round);
    void revertColor();
    void showHint();

    // evaluate function for AI
    bool hasNeighbor(int x, int y, int delta, int count);
    QVector<Piece> getCandidate();
    int evaluate(Piece::PieceColor color);
    int getPattern(Piece::PieceColor, int srcX, int srcY, int deltaX, int deltaY);
    int calScore(unsigned short pat[]);
    void ai(Piece::PieceColor color);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void piecePlaced(int row, int col, Piece::PieceColor color);
    void gameOver(Piece::PieceColor color);

private:
    bool checkWin(int row, int col, Piece::PieceColor color);
    bool isOnBoard(int x, int y)
    {
        return 0 <= x && x <= Const::SIZE && 0 <= y && y <= Const::SIZE;
    }
    bool isAvailable(int x, int y)
    {
        return isOnBoard(x, y) && m_board[x][y].getColor() == Piece::Transparent;
    }
    bool hasBomb(int x, int y);

    Piece m_board[Const::SIZE + 1][Const::SIZE + 1];
    QPointF m_center;
    Piece::PieceColor m_color;
    bool m_is_hidden;
    bool m_is_blocked;
    int m_round;
    int m_my_pieces;
    double m_cell_width;

    // for undo
    QStack<Piece> m_stack;

    // for hint
    QTimer m_bomb_timer;
    bool m_bomb[Const::SIZE + 1][Const::SIZE + 1];
};

#endif
