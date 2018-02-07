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
    enum Mode
    {
        Single = 0,
        Network,
        AI
    };

    explicit Gomoku(QMainWindow *parent = 0);
    ~Gomoku();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void onTimeOut();
    void onChooseColor();
    void onPause();
    void onContinue();
    void onMyMove(int row, int col, Piece::PieceColor color);
    void setMode(int mode);
    void onGameOver(Piece::PieceColor color);

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
    Mode m_mode;
    Piece::PieceColor m_color;
    QTimer  m_timer;
    int m_time_left;
    int m_opp_tot_time;
    int m_my_tot_time;
    int m_black_time;
    int m_white_time;
    bool m_is_started;
    bool m_is_blocked;

};

#endif
