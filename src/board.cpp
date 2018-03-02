#include "board.h"

#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>

Board::Board(QWidget *parent) :
    QWidget(parent),
    m_color(Piece::White),
    m_is_hidden(false),
    m_is_blocked(false),
    m_round(0),
    m_my_pieces(0)
{
    this->setMouseTracking(true);
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++) m_bomb[i][j] = 0;
    connect(&m_bomb_timer, &QTimer::timeout, this, [this]()
    {
        for (int i = 0; i <= Const::SIZE; i++)
            for (int j = 0; j <= Const::SIZE; j++) m_bomb[i][j] = 0;
        this->update();
    });
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
                painter.drawPixmap(center.x() - r, center.y() - r, r * 2, r * 2, QPixmap(":/icons/bomb.png"));
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
    if (m_is_blocked)
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
    if (m_is_blocked || event->button() != Qt::LeftButton) return;
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            if (m_board[i][j].getState() == Piece::Hover)
            {
                int halfSize = Const::SIZE / 2, r = m_cell_width / 2;
                QPointF center = m_center + QPointF((i - halfSize) * m_cell_width, (j - halfSize) * m_cell_width);
                if (Const::Sqr(center.x() - event->x()) + Const::Sqr(center.y() - event->y()) >= r * r) continue;
                if (event->button() == Qt::LeftButton)
                {
                    m_my_pieces++;
                    placePiece(i, j, m_color);
                    emit piecePlaced(i, j, m_color);
                    return;
                }
            }
}

void Board::clear()
{
    m_round = 0;
    m_my_pieces = 0;
    for (int i = 0 ; i <= Const::SIZE; i++)
        for (int j = 0 ; j <= Const::SIZE; j++) m_board[i][j] = Piece();
    this->update();
}

void Board::revertColor()
{
    if (m_color == Piece::White)
        m_color = Piece::Black;
    else
        m_color = Piece::White;
}

void Board::placePiece(int row, int col, Piece::PieceColor color)
{
    if (row < 0 || col < 0 || color == Piece::Transparent) return;
    m_board[row][col] = Piece(row, col, color, ++m_round);
    m_stack.push(m_board[row][col]);
    this->update();
    if (checkWin(row, col, color))
    {
        emit gameOver(color);
    }
    else if (m_round == Const::SIZE * Const::SIZE)
    {
        emit gameOver(Piece::Transparent);
    }
}

void Board::undo(int round)
{
    for (int i = 0; i < round && m_stack.size(); i++)
    {
        Piece piece = m_stack.top();
        m_board[piece.getRow()][piece.getCol()].setState(Piece::None);
        m_board[piece.getRow()][piece.getCol()].setColor(Piece::Transparent);
        m_stack.pop();
        m_round--;
        if (piece.getColor() == m_color)
            m_my_pieces--;
    }
    this->update();
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

bool Board::hasBomb(int x, int y)
{
    static const int dir[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
                                //  --      |       \        /
    if (m_board[x][y].getColor() != Piece::Transparent) return false;
    Piece::PieceColor oppColor = m_color == Piece::Black ? Piece::White : Piece::Black;
    m_board[x][y].setColor(oppColor);

    int n3 = 0, n4 = 0, n4_ = 0, n5 = 0;
    for (int i = 0; i < Const::SIZE; i++)
        for (int j = 0; j < Const::SIZE; j++)
            if (m_board[i][j].getColor() == oppColor)
            {
                for (int d = 0; d < 4; d++)
                {
                    int ii = i, jj = j, k;
                    for (k = 0; k < 5 && isOnBoard(ii, jj) && m_board[ii][jj].getColor() == oppColor; k++, ii += dir[d][0], jj += dir[d][1]);
                    if (k == 3 && isAvailable(ii,jj) && isAvailable(i - dir[d][0], j - dir[d][1])) n3++;
                    if (k == 4 && isAvailable(ii, jj) && isAvailable(i - dir[d][0], j - dir[d][1])) n4_++;
                    if (k == 4 && (isAvailable(ii, jj) || isAvailable(i - dir[d][0], j - dir[d][1]))) n4++;
                    if (k == 5) n5++;

                    if (n3 >= 2 || (n3 && n4) || n4_ || n5)
                    {
                        m_board[x][y].setColor(Piece::Transparent);
                        return true;
                    }
                }
            }
    m_board[x][y].setColor(Piece::Transparent);
    return false;
}

void Board::showHint()
{
    m_bomb_timer.setSingleShot(true);
    m_bomb_timer.start(2000);
    for (int i = 0; i <= Const::SIZE; i++)
        for (int j = 0; j <= Const::SIZE; j++)
            m_bomb[i][j] = hasBomb(i, j);
    this->update();
}

bool Board::hasNeighbor(int x, int y, int delta, int count)
{
    int startX = x - delta;
    int startY = y - delta;
    int endX = x + delta;
    int endY = y + delta;
    for (int i = startX; i <= endX; i++)
    {
        for (int j = startY; j <= endY; j++)
        {
            if (!isOnBoard(i, j) || (i == x && j == y))
                continue;
            if (m_board[i][j].getColor() != Piece::Transparent)
            {
                count--;
                if (count <= 0)
                    return true;
            }

        }
    }
    return false;
}
QVector<Piece> Board::getCandidate()
{
    QVector<Piece> candi;
    for (int i = 0; i <= Const::SIZE; i++)
    {
        for (int j = 0; j <= Const::SIZE; j++)
        {
            if (m_board[i][j].getColor() == Piece::Transparent)
            {
                if (hasNeighbor(i, j, 1, 1))
                {
                    candi.push_back(Piece(i,j));
                    qDebug() << "candidates point: (" << i << "," << j << ")";
                }
            }
        }
    }
    return candi;
}

/*
 */
int Board::evaluate(Piece::PieceColor color)
{
    static const int dir[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
                                //  --      |       \        /
    int score = 0;
    unsigned short pattern[70] = { 0 };

    for (int i = 0; i <= Const::SIZE; i++)
    {
        for (int j = 0; j < Const::SIZE; j++)
        {
            if (m_board[i][j].getColor() != color)
                continue;
            for (int k = 0; k < 4; k++)
            {
                int curPat = getPattern(color, i, j, dir[k][0], dir[k][1]);
                if (curPat > 0)
                {
                    ++pattern[curPat];
                }
                else if (curPat < 0)
                {
                    curPat = -curPat;
                    int nr = i + dir[k][0] * (curPat - 1);
                    int nc = j + dir[k][1] * (curPat - 1);
                    while (m_board[nr][nc].getColor() != color) {
                        nr -= dir[k][0];
                        nc -= dir[k][1];
                    }
                    curPat = getPattern(color, nr, nc, -dir[k][0], -dir[k][1]);
                    if (curPat > 0)
                        ++pattern[curPat];
                }
            }
        }
    }
    score = calScore(pattern);
    return score;
}

int Board::getPattern(Piece::PieceColor color, int srcX, int srcY, int deltaX, int deltaY)
{
    int tr = srcX - deltaX;
    int tc = srcY - deltaY;

    bool live = true;
    if (!isOnBoard(tr, tc))
    {
        live = false;
    }
    else
    {
        if (m_board[tr][tc].getColor() == color)
            return 0;
        live = (m_board[tr][tc].getColor() == Piece::Transparent) ? true : false;
    }

    unsigned short curPat = 1;
    for (int i = 1; i < 5; i++)
    {
        curPat <<= 1;
        tr = srcX + deltaX * i;
        tc = srcY + deltaY * i;
        if (!isOnBoard(tr, tc))
            return -i;
        else if (m_board[tr][tc].getColor() == Piece::Transparent)
            curPat |= 0;
        else if (m_board[tr][tc].getColor() == color)
            curPat |= 1;
        else
            return -i;
    }
    curPat |= (live ? 0 : 1) << 5;
    return curPat;
}

/*
 * Calculate score based on the pattern of current board.
 * the pattern is an unsigned short, only 6 bits are valid.
 * the most significant bit stands for liveness, 1 for dead, 0 for live.
 * for other bits, 1 for piece, 0 for no piece.
 *
 * Pattern are list below:
 *  0 11111(31)         =>  FIVE
 *  1 11111(63)         =>  FIVE
 *  0 11110(30)         =>  FOUR
 *  1 11110(62) * 2     =>  DOUBLE_SLEEP_FOUR
 *  1 11110 & 0 11100   =>  SLEEP_FOUR & THREE
 *  0 11100(28)         =>  DOUBLE_THREE
 *  1 11100 & 0 11100   =>  SLEEP_THREE & THREE
 *  1 11110(62)         =>  SLEEP_FOUR
 *  0 11100(28)         =>  THREE
 *  0 11000(24) * 2     =>  DOUBLE_TWO
 *  1 11100(60)         =>  SLEEP_THREE
 *  0 11000 & 1 11000   =>  SLEEP_TWO & TWO
 *  0 11000(24)         =>  TWO
 *  1 11000(56)         =>  SLEEP_TWO
 */
int Board::calScore(unsigned short pat[])
{
    struct SNode {
        int score;
        int req[2][2];
        SNode() : score(0) { req[0][0] = req[1][0]=-1; }
        SNode(int sc, int pat1, int cnt1, int pat2=-1, int cnt2=-1) {
            score = sc;
            req[0][0] = pat1;
            req[0][1] = cnt1;
            req[1][0] = pat2;
            req[1][1] = cnt2;
        }
    };

    SNode scale[] = {   // 分值表
        SNode(Const::Score::FIVE,                   63, 1),
        SNode(Const::Score::FIVE,                   31, 1),
        SNode(Const::Score::FOUR,                   30, 1),
        SNode(Const::Score::DOUBLE_SLEEP_FOUR,      62, 2),
        SNode(Const::Score::SLEEP_FOUR_AND_THREE,   62, 1, 28, 1),
        SNode(Const::Score::DOUBLE_THREE,           28, 2),
        SNode(Const::Score::SLEEP_THREE_AND_THREE,  60, 1, 28, 1),
        SNode(Const::Score::SLEEP_FOUR,             62, 1),
        SNode(Const::Score::THREE,                  28, 1),
        SNode(Const::Score::DOUBLE_TWO,             24, 2),
        SNode(Const::Score::SLEEP_THREE,            60, 1),
        SNode(Const::Score::TWO,                    24, 1),
        SNode(Const::Score::SLEEP_TWO,              56, 1)
    };

    bool flag = true;
    for (int i = 0; i < sizeof(scale)/sizeof(scale[0]); i++)
    {
        SNode cur = scale[i];

        flag = true;
        for (int j = 0; j < 2; ++j)
        {
            if (cur.req[j][0] == -1)
                continue;
            if (pat[cur.req[j][0]] <  cur.req[j][1])
                flag = false;
        }
        if (flag)
            return cur.score;
    }
    return 0;
}

void Board::ai(Piece::PieceColor color)
{
    int best =0;
    int score = 0;
    Piece bestMove;
    QVector<Piece> candidates = getCandidate();
    qDebug() << "getCandidates" << candidates.size();

    foreach (Piece candidate, candidates) {
        int x = candidate.getRow();
        int y = candidate.getCol();
        m_board[x][y].setColor(color);
        score = Const::nodeValue[x][y] + evaluate(color);
        m_board[x][y].setColor(Piece::Transparent);
        qDebug() << "reset Move score is :" << score;

        if (best < score)
        {
            best = score;
            bestMove = candidate;
        }
    }
    placePiece(bestMove.getRow(), bestMove.getCol(), color);
}
