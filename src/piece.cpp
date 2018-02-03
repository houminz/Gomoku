#include "piece.h"

Piece::Piece() :
    m_color(Transparent), m_state(None)
{

}

Piece::Piece(int row, int col, PieceColor color) :
    m_row(row), m_col(col), m_color(color), m_state(Placed)
{

}

Piece::~Piece()
{

}

