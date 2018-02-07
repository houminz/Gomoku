#ifndef GOMOKU_H
#define GOMOKU_H

#include "piece.h"

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class Gomoku;
}

class Gomoku : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gomoku(QMainWindow *parent = 0);
    ~Gomoku();

private slots:
    void onChooseColor();
    void onPause();
    void onContinue();
    void onMyMove(int row, int col, Piece::PieceColor color);

    void start();
    void pause();
    void drop();
    void undo();
    void hint();
    void disconnect();
    void about();
    void exit();

signals:
    void disconnected();

private:
    Ui::Gomoku *ui;
    QTimer  *m_timer;
    bool m_is_started;
    bool m_is_blocked;
};

#endif
