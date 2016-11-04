#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <QMainWindow>

class QMediaPlayer;
class QMediaPlaylist;

namespace Ui {
class MyPlayer;
}

class MyPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyPlayer(QWidget *parent = 0);
    ~MyPlayer();

private:
    Ui::MyPlayer *ui;

    //player core
    QMediaPlayer *player;
    QMediaPlaylist *playList;

    //control
    int volume;
};

#endif // MYPLAYER_H
