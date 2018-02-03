#ifndef GOMOKU_H
#define GOMOKU_H

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
    void start();
    void pause();
    //void disconnect();

private:
    Ui::Gomoku *ui;
    QTimer  *timer;
};

#endif
