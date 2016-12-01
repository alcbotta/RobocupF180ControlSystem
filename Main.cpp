#include "./src/GameMainWindow.h"
#include <QApplication>

#include "./src/old/Element.h"
#include "./src/old/Point.h"
#include "./src/old/UtilGame.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameMainWindow game;
    game.show();

    return a.exec();
}
