#include "gomoku.h"
#include "ui_gomoku.h"
#include "const.h"

Gomoku::Gomoku(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Gomoku),
    timer(new QTimer)
{
    ui->setupUi(this);

    //connect(timer, &QTimer::timeout, this, dsa);
    connect(ui->start, &QPushButton::clicked, this, &Gomoku::start);
    connect(ui->pause, &QPushButton::clicked, this, &Gomoku::pause);
    //connect(ui->disconnect, &QPushButton::clicked, this, Gomoku::disconnect);

}

Gomoku::~Gomoku()
{
    delete ui;
}

void Gomoku::start()
{
    timer->start(Const::TIME_LIMIT);
}

void Gomoku::pause()
{
    if (timer->isActive())
        timer->stop();
}
