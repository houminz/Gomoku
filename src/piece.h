#ifndef PIECE_H
#define PIECE_H

class Piece
{
public:
    explicit Piece();
    explicit Piece(int row, int col);
    ~Piece();

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

    // Getter member functions
    int getRow() const { return m_row; }
    int getCol() const { return m_col; }
    PieceColor getColor() const { return m_color; }
    PieceState getState() const { return m_state; }

    // Setter member functions
    void setColor(PieceColor color) { m_color = color; }
    void setState(PieceState state) { m_state = state; }

private:
    int m_row;
    int m_col;
    PieceColor m_color;
    PieceState m_state;
};

#endif // PIECE_H
