#include "myplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("fusion");
    MyPlayer w;

    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
