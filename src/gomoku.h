#ifndef GOMOKU_H
#define GOMOKU_H

#include <QMainWindow>

namespace Ui {
class Gomoku;
}

class Gomoku : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gomoku(QMainWindow *parent = 0);
    ~Gomoku();

private:
    Ui::Gomoku *ui;
};

#endif
