#include <QApplication>
#include <QTranslator>

#include "gomoku.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Houmin");
    QCoreApplication::setApplicationName("Gomoku");

    QTranslator trans1;
    trans1.load(":/trans/zh_CN.qm");
    qApp->installTranslator(&trans1);

    qRegisterMetaType<Piece::PieceColor>("Piece::PieceColor");

    Gomoku game;
    game.show();

    return app.exec();
}
