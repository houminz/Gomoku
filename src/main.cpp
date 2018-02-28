#include <QApplication>

#include "gomoku.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Houmin");
    QCoreApplication::setApplicationName("Gomoku");

    qRegisterMetaType<Piece::PieceColor>("Piece::PieceColor");

    Gomoku game;
    game.show();

    return app.exec();
}
