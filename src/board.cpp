#include "board.h"

#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QtDebug>

Board::Board(QWidget *parent) :
    QWidget(parent),
    m_color(Piece::White),
    m_round(0)
{
    this->setMouseTracking(true);

}

Board::~Board()
{

}

void Board::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    m_cell_width = 1.0 * std::min(this->width(), this->height()) / (Const::SIZE + 2) * 0.95;
    m_center = QPointF(this->width() / 2, this->height() / 2);
}

void Board::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(m_center);
    int halfSize = Const::SIZE / 2;
    double halfWidth = Const::SIZE * m_cell_width / 2;
    double frameWidth = (Const::SIZE + 2) * m_cell_width;

    QPen pen(Qt::black);
    pen.setWidth(2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(pen);
    painter.setBrush(QColor(255, 255, 170));
    if (m_is_hidden) painter.setBrush(Qt::transparent);
    painter.drawRect(-frameWidth / 2, -frameWidth / 2, frameWidth, frameWidth);

    // board grid
    painter.setPen(Qt::black);
    for (int i = -halfSize; i <= halfSize; i++)
    {
        painter.drawLine(-halfWidth, i * m_cell_width, halfWidth, i * m_cell_width);
        painter.drawLine(i * m_cell_width, -halfWidth, i * m_cell_width, halfWidth);
    }

    // base point
    double r = m_cell_width / 5;
    painter.setBrush(Qt::black);
    for (int i = 0; i < 5; i++)
        painter.drawEllipse(Const::POINT[i][0] * m_cell_width - r/2, Const::POINT[i][1] * m_cell_width - r/2, r, r);

    // pieces distribution
    if (m_is_hidden) return;
    r = m_cell_width / 2;
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
        {
            QPointF center((i - halfSize) * m_cell_width , (j - halfSize) * m_cell_width);
            if (m_bomb[i][j])
                painter.drawPixmap(center.x() - r, center.y() - r, r * 2, r * 2, QPixmap(":/icon/icon/bomb.png"));
            if (m_board[i][j].getState() != Piece::None)
            {
                Piece piece = m_board[i][j];
                QColor color = Const::PIECE_COLOR[piece.getColor()];
                if (piece.getState() == Piece::Hover)
                {
                    color = Const::PIECE_COLOR[m_color];
                    color.setAlpha(150);
                }
                painter.setBrush(color);
                painter.setPen(Qt::black);
                painter.drawEllipse(center, r, r);

                // draw dash line for just placed piece
                if (piece.getRound() == m_round && piece.getState() == Piece::Placed)
                {
                    pen.setColor(Qt::red);
                    pen.setStyle(Qt::DashLine);
                    painter.setPen(pen);
                    painter.setBrush(Qt::transparent);
                    painter.drawRect(center.x() - r, center.y() - r, r * 2, r * 2);
                }
            }
        }
}
void Board::mouseMoveEvent(QMouseEvent* event)
{
    if (m_is_block)
    {
        for (int i = 0; i <= Const::SIZE; i++)
            for (int j = 0; j <= Const::SIZE; j++)
                if (m_board[i][j].getState() == Piece::Hover)
                    m_board[i][j].setState(Piece::None);
        this->update();
        return;
    }
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            if (m_board[i][j].getState() != Piece::Placed)
            {
                int halfSize = Const::SIZE / 2, r = m_cell_width / 2;
                QPointF center = m_center + QPointF((i - halfSize) * m_cell_width, (j - halfSize) * m_cell_width);
                if (Const::Sqr(center.x() - event->x()) + Const::Sqr(center.y() - event->y()) >= r * r)
                    m_board[i][j].setState(Piece::None);
                else
                    m_board[i][j].setState(Piece::Hover);
            }
    this->update();
    return;
}
void Board::mousePressEvent(QMouseEvent* event)
{
    if (m_is_block || event->button() != Qt::LeftButton) return;
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            if (m_board[i][j].getState() == Piece::Hover)
            {
                int halfSize = Const::SIZE / 2, r = m_cell_width / 2;
                QPointF center = m_center + QPointF((i - halfSize) * m_cell_width, (j - halfSize) * m_cell_width);
                if (Const::Sqr(center.x() - event->x()) + Const::Sqr(center.y() - event->y()) >= r * r) continue;
                if (event->button() == Qt::LeftButton)
                {
                    //m_my_pieces++;
                    //emit piecePlaced(i, j, m_color);
                    placePiece(i, j, m_color);
                    return;
                }
            }
}

void Board::placePiece(int row, int col, Piece::PieceColor color)
{
    if (row < 0 || col < 0 || color == Piece::Transparent) return;
    m_board[row][col] = Piece(row, col, color, ++m_round);
    //m_stack.push(m_board[row][col]);
    this->update();
    if (checkWin(row, col, color))
    {
        emit gameOver();
        if (color == m_color)
            QMessageBox::information(this, tr("WIN!"), tr("Congratulation! You win the game :-)"));
        else
            QMessageBox::information(this, tr("LOSE"), tr("You lose the game :-("));
    }
    else if (m_round == Const::SIZE * Const::SIZE)
    {
        emit gameOver();
        QMessageBox::information(this, tr("DRAW"), tr("2333333333..."));
    }
}
bool Board::checkWin(int row, int col, Piece::PieceColor color)
{
    for (int i = 0, j; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; j <= Const::SIZE && m_board[j][col].getColor() == color; j++);
        if (j - i >= 5) return true;
    }
    for (int i = 0, j; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; j <= Const::SIZE && m_board[row][j].getColor() == color; j++);
        if (j - i >= 5) return true;
    }
    for (int i = 0, j, k; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; k = j - row + col, 0 <= k && k <= Const::SIZE && m_board[j][k].getColor() == color; j++);
        if (j - i >= 5) return true;
    }
    for (int i = 0, j, k; i <= Const::SIZE; i = j + 1)
    {
        for (j = i; k = row + col - j, 0 <= k && k <= Const::SIZE && m_board[j][k].getColor() == color; j++);
        if (j - i >= 5) return true;
    }
    return false;
}

