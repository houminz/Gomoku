#ifndef PIECE_H
#define PIECE_H

class Piece
{
public:
    enum PieceColor
    {
        Transparent,
        White,
        Black
    };
    enum PieceState
    {
        None,
        Hover,
        Placed
    };

    explicit Piece();
    explicit Piece(int row, int col);
    explicit Piece(int row, int col, PieceColor color, int round);
    ~Piece();

    // Getter member functions
    int getRow() const { return m_row; }
    int getCol() const { return m_col; }
    int getRound() const { return m_round; }
    PieceColor getColor() const { return m_color; }
    PieceState getState() const { return m_state; }

    // Setter member functions
    void setColor(PieceColor color) { m_color = color; }
    void setState(PieceState state) { m_state = state; }
    void setPostion(int x, int y) { m_row = x; m_col = y; }

private:
    int m_row;
    int m_col;
    int m_round;
    PieceColor m_color;
    PieceState m_state;
};

#endif // PIECE_H
