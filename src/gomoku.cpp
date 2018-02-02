#include "gomoku.h"
#include "ui_gomoku.h"

Gomoku::Gomoku(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gomoku)
{
    ui->setupUi(this);
}

Gomoku::~Gomoku()
{
    delete ui;
}
