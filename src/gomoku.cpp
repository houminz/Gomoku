#include "gomoku.h"
#include "ui_gomoku.h"

Gomoku::Gomoku(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Gomoku)
{
    ui->setupUi(this);
 /*   ui->centralWidget->setStyleSheet(
                "background-image : url(\":/images/city.jpg\");"
                "background-position : center;"
                );
                */
}

Gomoku::~Gomoku()
{
    delete ui;
}

