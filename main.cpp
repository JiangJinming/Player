#include "myplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyPlayer w;
    w.show();

    return a.exec();
}
