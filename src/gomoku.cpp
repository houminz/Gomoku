#include "gomoku.h"
#include "ui_gomoku.h"

#include <QtDebug>
#include <QTime>
#include <QMessageBox>

Gomoku::Gomoku(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Gomoku),
    m_mode(Gomoku::Single),
    m_is_blocked(false),
    m_color(Piece::Black),
    m_black_time(0),
    m_white_time(0),
    m_my_tot_time(0),
    m_opp_tot_time(0)
{
    ui->setupUi(this);

    ui->lcd_left0->display("00");
    ui->lcd_left1->display("00");
    ui->lcd_total0->display("00:00");
    ui->lcd_total1->display("00:00");

    connect(ui->start, &QPushButton::clicked, this, &Gomoku::start);
    connect(ui->pause, &QPushButton::clicked, this, &Gomoku::pause);
    connect(ui->drop, &QPushButton::clicked, this, &Gomoku::drop);
    connect(ui->undo, &QPushButton::clicked, this, &Gomoku::undo);
    connect(ui->hint, &QPushButton::clicked, this, &Gomoku::hint);
    connect(ui->disconnect, &QPushButton::clicked, this, &Gomoku::disconnect);

    connect(ui->about, &QAction::triggered, this, &Gomoku::about);
    connect(ui->exit, &QAction::triggered, this, &Gomoku::exit);

    connect(ui->mode, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)));


    connect(ui->board, &Board::piecePlaced, this, &Gomoku::onMyMove);
    connect(ui->board, &Board::gameOver, this, &Gomoku::onGameOver);

    connect(&m_timer, &QTimer::timeout, this, &Gomoku::onTimeOut);

    initialize();
}

Gomoku::~Gomoku()
{
    delete ui;
}

void Gomoku::closeEvent(QCloseEvent* event)
{
    qDebug() << "receive close event";
    emit disconnected();
    qApp->exit(0);
}

void Gomoku::initialize()
{
    //setBlock(true);
    ui->board->setHidden(false);

    ui->drop->setEnabled(false);
    ui->start->setEnabled(false);
    ui->pause->setEnabled(false);
//    if (m_type == Const::Server)
//    {
//        ui->ip0->setText(QString("%1:%2").arg(Const::GetLocalIp()).arg(m_port));
//        ui->ip1->setText("");
//        ui->player0->setTitle(m_username);
//        ui->player1->setTitle(tr("Player1"));
//    }
//    else if (m_type == Const::Client)
//    {
//        ui->ip0->setText("");
//        ui->ip1->setText("");
//        ui->player0->setTitle(tr("Player0"));
//        ui->player1->setTitle(m_username);
//    }

}

void Gomoku::setMode(int mode)
{
    if (QMessageBox::question(this, tr("Ret Mode"), tr("Do you really want to change the game mode?")) == QMessageBox::Yes)
    {
        qDebug() << "set Mode" << mode;
        switch(mode)
        {
            case 0: m_mode = Gomoku::Single;    break;
            case 1: m_mode = Gomoku::Network;   break;
            case 2: m_mode = Gomoku::AI;        break;
            default: break;
        }
        ui->board->clear();
    }
}

void Gomoku::onGameOver(Piece::PieceColor color)
{
    if (color == Piece::Transparent)
        QMessageBox::information(this, tr("DRAW"), tr("2333333333..."));
    else
        switch (m_mode)
        {
        case Gomoku::AI:
        case Gomoku::Network:
            if (color == m_color)
                QMessageBox::information(this, tr("WIN!"), tr("You win the game :-)"));
            else if (color == Piece::Black)
                QMessageBox::information(this, tr("LOSE"), tr("You lose the game :-("));
            break;
        case Gomoku::Single:
            if (color == Piece::White)
                QMessageBox::information(this, tr("WIN!"), tr("White win the game :-)"));
            else if (color == Piece::Black)
                QMessageBox::information(this, tr("LOSE"), tr("Black win the game :-)"));
            break;
        default:
            break;
        }

    ui->board->setBlock(true);
}

void Gomoku::onGameStartPrepare()
{
    m_timer.stop();
    //if (m_is_choosing_color) return;
    //this->setBlock(true);
    m_is_started = false;
    ui->start->setText(tr("&Start"));
    ui->pause->setEnabled(false);
    ui->undo->setEnabled(false);
    ui->drop->setEnabled(false);
}

void Gomoku::onTimeOut()
{
    m_time_left--;
    QString time = QString::number(m_time_left);
    if (m_time_left < 10) time = "0" + time;
    if (ui->board->getColor() == Piece::Black)
    {
        ui->lcd_total0->display(QTime(0, 0, 0).addSecs(++m_black_time).toString("mm:ss"));
        ui->lcd_left0->display(time);
        ui->lcd_total1->display(QTime(0, 0, 0).addSecs(m_white_time).toString("mm:ss"));
        ui->lcd_left1->display(Const::TIME_LIMIT);
        ui->lcd_left1->setStyleSheet("");
        if (m_time_left <= 10)
            ui->lcd_left0->setStyleSheet("color:red;");
        else
            ui->lcd_left0->setStyleSheet("");
    }
    else
    {
        ui->lcd_total0->display(QTime(0, 0, 0).addSecs(m_black_time).toString("mm:ss"));
        ui->lcd_left0->display(Const::TIME_LIMIT);
        ui->lcd_left0->setStyleSheet("");
        ui->lcd_total1->display(QTime(0, 0, 0).addSecs(++m_white_time).toString("mm:ss"));
        ui->lcd_left1->display(time);
        if (m_time_left <= 10)
            ui->lcd_left1->setStyleSheet("color:red;");
        else
            ui->lcd_left1->setStyleSheet("");
    }

}

void Gomoku::onChooseColor()
{
    ui->board->clear();


}

void Gomoku::onContinue()
{

}

void Gomoku::onPause()
{
    if (m_timer.isActive())
        m_timer.stop();
}

void Gomoku::onMyMove(int row, int col, Piece::PieceColor color)
{
    ui->board->revertColor();
    m_time_left = Const::TIME_LIMIT + 1;

}

void Gomoku::start()
{
    qDebug() << "start now";
    m_time_left = Const::TIME_LIMIT + 1;
    m_timer.start(1000);
    if (m_is_started)
    {
        ui->start->setEnabled(false);
        onContinue();
    }
    else
    {
        onChooseColor();
    }
}

void Gomoku::pause()
{
    ui->start->setEnabled(true);
    onPause();
}


void Gomoku::undo()
{
    if (QMessageBox::question(this, tr("Undo Step"), tr("Do you want to undo 1 step")) == QMessageBox::Yes)
    {
        switch (m_mode)
        {
        case Gomoku::AI:
        case Gomoku::Single:
            ui->board->undo(1);
            ui->board->revertColor();
            break;
        case Gomoku::Network:
            ui->board->undo(1);
            break;
        default:
            break;
        }

        qDebug() << "Send undo to opponent";
    }
}

void Gomoku::hint()
{
    qDebug() << "Show Hint";
    ui->board->showHint();
}

void Gomoku::drop()
{
    if (QMessageBox::question(this, tr("Drop Game"), tr("Do you really want to drop the game?")) == QMessageBox::Yes)
    {
        QMessageBox::information(this, tr("LOSE"), tr("You drop the game :("));
    }
}

void Gomoku::disconnect()
{
    if (QMessageBox::question(this, tr("Disconnect"), tr("Do you really want to disconnect?")) == QMessageBox::Yes)
        emit disconnected();
}

void Gomoku::about()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About Gomoku"));
    msgBox.setIconPixmap(QPixmap(":/icons/gomoku.ico"));
    msgBox.setText(QString(tr(
                      "<h2>Gomoku Cosmos 0.01<br/></h2>"
                      "<p>PKU Embeded System Project</p>"
                      "<p>Based on Qt 5.10.0<br/></p>"
                      "<p>Version: %1</p>"
                      "<p>Built time: %2 - %3<br/></p>"
                      "<p>Copyright © 2018 Houmin Wei, Peking University. All Right Reserved.<br/></p>"
                      "<p>Project Index: <a href=\"https://github.com/SimpCosm/Gomoku\">https://github.com/SimpCosm/Gomoku</a></p>"
                      )).arg("1.0.0").arg(__DATE__).arg(__TIME__));
    msgBox.exec();
}

void Gomoku::exit()
{
    emit disconnected();
    qApp->exit(0);
}
