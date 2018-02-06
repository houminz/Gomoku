#include "gomoku.h"
#include "ui_gomoku.h"
#include "const.h"

#include <QtDebug>
#include <QMessageBox>

Gomoku::Gomoku(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Gomoku),
    m_timer(new QTimer),
    m_is_blocked(false)
{
    ui->setupUi(this);

    connect(ui->start, &QPushButton::clicked, this, &Gomoku::start);
    connect(ui->pause, &QPushButton::clicked, this, &Gomoku::pause);
    connect(ui->drop, &QPushButton::clicked, this, &Gomoku::drop);
    connect(ui->undo, &QPushButton::clicked, this, &Gomoku::undo);
    connect(ui->hint, &QPushButton::clicked, this, &Gomoku::hint);
    connect(ui->disconnect, &QPushButton::clicked, this, &Gomoku::disconnect);

    connect(ui->about, &QAction::triggered, this, &Gomoku::about);
    connect(ui->exit, &QAction::triggered, this, &Gomoku::exit);
}

Gomoku::~Gomoku()
{
    delete ui;
}

void Gomoku::onChooseColor()
{

}

void Gomoku::onContinue()
{

}

void Gomoku::onPause()
{
    if (m_timer->isActive())
        m_timer->stop();
}

void Gomoku::start()
{
    qDebug() << "start now";
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
    int undoStep = m_is_blocked ? 1 : 2;
    if (QMessageBox::question(this, tr("Undo Step"), QString(tr("Do you want to undo before %1 step%2?")).arg(undoStep).arg(undoStep == 1 ? "" : "s")) == QMessageBox::Yes)
    {
        qDebug() << "Send undo to opponent";
    }
}

void Gomoku::hint()
{
    qDebug() << "Show Hint";
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
