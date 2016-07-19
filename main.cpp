#include "wuziPlayboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//------------------------------------------------------------

    WuziPlayBoard board;
    board.show();

//------------------------------------------------------------

//    MainWindow w;
//    w.show();

    return a.exec();
}
