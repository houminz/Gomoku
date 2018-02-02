#include <QApplication>

#include "gomoku.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Houmin");
    QCoreApplication::setApplicationName("Notepad");

    Gomoku game;
    game.show();

    return app.exec();
}
