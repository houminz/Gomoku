#include "piece.h"

Piece::Piece() :
    m_color(Transparent), m_state(None)
{

}

Piece::Piece(int row, int col, PieceColor color, int round) :
    m_row(row), m_col(col), m_round(round), m_color(color), m_state(Placed)
{

}

Piece::~Piece()
{

}

